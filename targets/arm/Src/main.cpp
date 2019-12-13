#include "main.h"
#include "STM_hal.h"
#include "ZUMO.h"
#include "uart_crossing.h"
// terminals
#include "command_terminal/Command.h"
#include "command_terminal/command_manager.h"
#include "window_terminal/window_manager.hpp"
// algorithms
#include "mean.h"
#include "PID.h"
// devices
#include "HC-SR04.h"
#include "MCP9700.h"
#include "BME280.h"
#include "MPU6050.h"
#include "KTIR0711S.h"
#include "DRV8833.h"
#include "encoder.h"

#include "WS2812B.h"

extern ADC_HandleTypeDef hadc1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern I2C_HandleTypeDef hi2c1;

void hal::enable_interrupts() {
    __enable_irq();
}

void hal::disable_interrupts() {
    __disable_irq();
}

STM32_PWM<1000> PWM_1_CH1(TIM1->CCR1);
STM32_PWM<1000> PWM_1_CH2(TIM1->CCR2);

STM32_PWM<1000> PWM_2_CH3(TIM2->CCR3);
STM32_PWM<1000> PWM_2_CH4(TIM2->CCR4);

STM32_GPIO fault(FAULT_GPIO_Port, FAULT_Pin);
STM32_GPIO_FAKE nSleep;

STM32_GPIO MOT_L_A(MOT_L_A_GPIO_Port, MOT_L_A_Pin);
STM32_GPIO MOT_L_B(MOT_L_A_GPIO_Port, MOT_L_B_Pin);

STM32_GPIO MOT_R_A(MOT_R_A_GPIO_Port, MOT_R_A_Pin);
STM32_GPIO MOT_R_B(MOT_R_A_GPIO_Port, MOT_R_B_Pin);

STM32_GPIO SENS_IR_ENABLE(SENS_IR_LED_GPIO_Port, SENS_IR_LED_Pin);

STM32_GPIO TRIG(TRIG_GPIO_Port, TRIG_Pin);

STM32_I2C IMU_I2C(hi2c1);

volatile uint16_t sensors[11];
volatile uint16_t *TEMP = &sensors[8];
volatile uint16_t *V_CURRENT_SENS = &sensors[9];
volatile uint16_t *V_BAT = &sensors[10];

ZUMO& zumo (void) {
    static DRV8833 motor_driver(PWM_1_CH1, PWM_1_CH2, PWM_2_CH3, PWM_2_CH4, nSleep, fault);

    static Encoder encoderL(MOT_L_A, MOT_L_B, 1);

    static Encoder encoderR(MOT_R_A, MOT_R_B, 1);

    static LineSensors <volatile uint16_t , 4095, 8>line_sensors(&sensors[0], SENS_IR_ENABLE);

    static STM32_GPIO LED1(LED1_GPIO_Port, LED1_Pin);

    static STM32_GPIO LED2(LED2_GPIO_Port, LED2_Pin);

    static HCSR04 hcsr04(TRIG);

    static MCP9700<uint16_t > mcp9700((uint16_t&)(*TEMP), 4095, 3.3f);

    static BME280 bme280(IMU_I2C, 0b1110110);

    static MPU6050 mpu6050 (IMU_I2C, 0x68);

    static ZUMO _zumo (motor_driver, encoderL, encoderR, line_sensors, LED1, LED2, hcsr04, mcp9700, bme280, mpu6050);

    return _zumo;
}

volatile bool print_flag = false;

STM32_UART UART1(huart1);
STM32_UART UART2(huart2);
STM32_UART UART3(huart3);

extern "C" int handle_usart1_interrupt() {
    return UART1.interrupt();
}

extern "C" int handle_usart2_interrupt() {
    return UART2.interrupt();
}

extern "C" int handle_usart3_interrupt() {
    return UART3.interrupt();
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        zumo().hcsr04.edge_detected();
    }
}

volatile bool refresh_values = false;
volatile bool _10Hz_flag = false;

Mean <uint16_t, 20>current_mean;




//GPIOC->BSRR
//
//    (GPIO_TypeDef *)(WS2812B_GPIO_Port)->BSRR = pin;
//    set_pin(WS2812B_GPIO_Port, WS2812B_Pin);
//    reset_pin(WS2812B_GPIO_Port, );
//}

WS2812B<36> WS_LEDS(hal::enable_interrupts, hal::disable_interrupts);

extern "C"
void Main() {
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensors, 11);

    HAL_TIM_Base_Start_IT(&htim3);

    hal::setup();

    for (uint8_t i = 0; i < 12; i++) {
        WS_LEDS.set_color(i, 250,0,0);
    }
    for (uint8_t i = 12; i < 24; i++) {
        WS_LEDS.set_color(i, 0,250,0);
    }
    for (uint8_t i = 24; i < 36; i++) {
        WS_LEDS.set_color(i, 0,0,250);

    }

    WS_LEDS.send();

    while(1) {


        if (print_flag) {
            print_flag = false;
//            zumo().LED1.toggle();
        }

        hal::loop();

        if (_10Hz_flag) {
            zumo().mcp9700.get_temperature();
            zumo().bme280.run_measurements();
            zumo().hcsr04.run_measurements();
            zumo().mpu6050.run_measurements();

            zumo().LED2.toggle();
            _10Hz_flag = false;

            VT::move_to(0, 30);
            VT::print("TEMP: ");
            VT::print(zumo().mcp9700.get_last_temperature());

            VT::move_to(0, 31);
            VT::print("CURRENT: ");
            current_mean.put_value((uint16_t)*V_CURRENT_SENS);
            VT::print(current_mean.get_value());
            VT::move_to(0, 32);
            VT::print("BAT: ");
            VT::print((int)*V_BAT);
        }
    }
}

extern "C"
void My_SysTick_Handler() {
    static uint_fast16_t dd = 0;
    dd++;
    if (dd == 200) {
        refresh_values = true;
        dd = 0;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim3) { // 10kHz
        zumo().hcsr04.ISR();
        zumo().encoderR.encoder10kHzTickISR();
        zumo().encoderL.encoder10kHzTickISR();

        static uint16_t divider_10Hz = 0;
        static uint16_t divider_100Hz = 0;

        if (++divider_100Hz == 100 ) {
            divider_100Hz = 0;
        }

        if (++divider_10Hz == 1000) { // 10Hz
            divider_10Hz = 0;
            _10Hz_flag = true;
        }
    }
}