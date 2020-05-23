//#include "hal.h"
//#include "ZUMO.hpp"
//#include "cores/UART_crossing.h"
#include "main.h"
#include "STM_hal.h"

//#include "STM_WS2812B.hpp"
//#include "UART.hpp"
//#include "commands_runner.h"
#include "ZUMO_impementation.hpp"

//extern ADC_HandleTypeDef hadc1;
//extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim15;

extern I2C_HandleTypeDef hi2c1;

void hal::enable_interrupts() {
    __enable_irq();
}

void hal::disable_interrupts() {
    __disable_irq();
}

//STM32_GPIO MOT_L_A(MOT_L_A_GPIO_Port, MOT_L_A_Pin);
//STM32_GPIO MOT_L_B(MOT_L_A_GPIO_Port, MOT_L_B_Pin);
//
//STM32_GPIO MOT_R_A(MOT_R_A_GPIO_Port, MOT_R_A_Pin);
//STM32_GPIO MOT_R_B(MOT_R_B_GPIO_Port, MOT_R_B_Pin);
//
//STM32_GPIO SENS_IR_ENABLE(SENS_IR_LED_GPIO_Port, SENS_IR_LED_Pin);
//
//STM32_GPIO TRIG(TRIG_GPIO_Port, TRIG_Pin);
//
//STM32_I2C IMU_I2C(hi2c1);

//volatile uint16_t sensors[11];
//volatile uint16_t *TEMP = &sensors[8];
//volatile uint16_t *V_CURRENT_SENS = &sensors[9];
//volatile uint16_t *V_BAT = &sensors[10];

//extern Comman_manager_template command_manager;

//ZUMO& zumo () {
//    static DRV8833 motor_driver(PWM_1_CH2N, PWM_1_CH3N, PWM_1_CH1N, PWM_15_CH1, nSleep, fault);
//
//    static Encoder encoderL(MOT_L_A, MOT_L_B, 1);
//
//    static Encoder encoderR(MOT_R_A, MOT_R_B, 1);
//
//    static LineSensors <volatile uint16_t , 4095, 8>line_sensors(&sensors[0], SENS_IR_ENABLE);
//
//    static STM32_GPIO LED1(LED1_GPIO_Port, LED1_Pin);
//
//    static STM32_GPIO LED2(LED2_GPIO_Port, LED2_Pin);
//
//    static HCSR04 hcsr04(TRIG);
//
//    static MCP9700<uint16_t > mcp9700((uint16_t&)(*TEMP), 4095, 3.3f);
//
//    static BME280 bme280(IMU_I2C, 0b1110110);
//
//    static MPU6050 mpu6050 (IMU_I2C, 0x68);
//
//    static STM_WS2812B<2> ws2812b(hal::enable_interrupts, hal::disable_interrupts);
//
//    static ZUMO _zumo(motor_driver, encoderL, encoderR, line_sensors, LED1, LED2, hcsr04, mcp9700, bme280, mpu6050, ws2812b);
//
//    return _zumo;
//}

volatile bool print_flag = false;

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
//        zumo().hcsr04.edge_detected();
    }
}

volatile bool refresh_values = false;
volatile bool _10Hz_flag = false;

//Mean <uint16_t, 20>current_mean;

extern "C"
void Main() {

    ZUMO& zumo = ZUMO::zumo();
    zumo.init();

//    zumo().init();
//    Uart uart1 = {USART1, 230400};
//    Uart uart2 = {USART2, 230400};
//    Uart uart3 = {USART3, 230400};
//    UART_crossing crossing;
//    crossing.addUart(&uart1);
//    crossing.addUart(&uart2);
//    crossing.addUart(&uart3);
//    crossing.init();
//    uart1.init();
//    uart2.init();
//    uart3.init();
//    uart1.write("siema", 5);
//    uart2.write("siema", 5);
//    uart3.write("siema", 5);

//    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensors, 11);

    HAL_TIM_Base_Start_IT(&htim2);
//    command_manager.init();
    while(true) {
        ZUMO::loop();


        if (print_flag) {
            print_flag = false;
//            zumo().LED1.toggle();
        }

//        hal::loop();

//        if (command_manager.run()){
//            zumo().LED1.toggle();
//        }
//        callbacks_runner(command_manager);


        if (_10Hz_flag) {
            // TODO
//            zumo().mcp9700.get_temperature();
//            zumo().bme280.run_measurements();
//            zumo().hcsr04.run_measurements();
//            zumo().mpu6050.run_measurements();
//
//             zumo().LED2.toggle();
            _10Hz_flag = false;
//            current_mean.put_value((uint16_t)*V_CURRENT_SENS);
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
    if (htim == &htim2) { // 10kHz
//        zumo().hcsr04.ISR();
//        zumo().encoderR.encoder10kHzTickISR();
//        zumo().encoderL.encoder10kHzTickISR();

        ZUMO::ISR_10kHz();
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