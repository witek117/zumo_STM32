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
void _10Hz();

Mean <uint16_t, 20>current_mean;

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
    zumo().LED2.toggle();
    zumo().mcp9700.init();
    zumo().bme280.init();
    zumo().mpu6050.init(MPU6050::GyroscopeData::Scale::DPS_2000, MPU6050::AccelerometerData::Range::G2);
    zumo().mpu6050.gyroscope.calibrate(5);

    while(1) {
        if (print_flag) {
            print_flag = false;
//            zumo().LED1.toggle();
        }

        hal::loop();

        if (_10Hz_flag) {
//
////            Vector rawAccel = zumo().mpu6050.readRawAccel();
//            auto raw_accel = zumo().mpu6050.accelerometer.get_raw_data();
//            auto normAccel = zumo().mpu6050.accelerometer.get_normalised_data();
//            int pitch = -(atan2(normAccel.x, sqrt(normAccel.y*normAccel.y + normAccel.z*normAccel.z))*180.0)/M_PI;
//            int roll = (atan2(normAccel.y, normAccel.z)*180.0)/M_PI;
//////            VT::move_to(0,33);
//////            VT::print(rawAccel.XAxis);
//////            VT::print(' ');
//////            VT::print(rawAccel.YAxis);
//////            VT::print(' ');
//////            VT::print(rawAccel.ZAxis);
//            VT::move_to(0,35);
//            VT::print(pitch);
//            VT::print(' ');
//            VT::print(roll);
//            VT::print("       ");
////
//            VT::move_to(0,33);
//            VT::print(raw_accel.x);
//            VT::print(' ');
//            VT::print(raw_accel.y);
//            VT::print(' ');
//            VT::print(raw_accel.z);
//            VT::print("        ");
//
//            VT::move_to(0,34);
//            VT::print(normAccel.x);
//            VT::print(' ');
//            VT::print(normAccel.y);
//            VT::print(' ');
//            VT::print(normAccel.z);
//            VT::print("        ");
////
//////            Vector rawGyro = zumo().mpu6050.readRawGyro();
//            auto normGyro = zumo().mpu6050.gyroscope.get_normalised_data();
////
//            VT::move_to(0,36);
//            VT::print(normGyro.x);
//            VT::print(' ');
//            VT::print(normGyro.y);
//            VT::print(' ');
//            VT::print(normGyro.z);
//            VT::print("        ");

            zumo().bme280.run_measurements();
            zumo().hcsr04.run_measurements();

            zumo().LED2.toggle();
            _10Hz_flag = false;
            _10Hz();

            VT::move_to(0, 30);
            VT::print("TEMP: ");
            VT::print(zumo().mpu6050.get_temperature());

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

void _10Hz() {
//    int R_data = -zumo().encoderR.encoderGetCountAndReset();
//    int L_data = zumo().encoderL.encoderGetCountAndReset();
//    Encoder_R.set(R_data);
//    Encoder_L.set(L_data);

//    float ret_R = pid_R.tick(static_cast<float>(R_data) / 400.0);
//    float ret_L = pid_L.tick(static_cast<float>(L_data) / 400.0);

//    Duty_R.set((int)(ret_R * 100.0));
//    Duty_L.set((int)(ret_L * 100.0));
}

//Mean<float, 5>position;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    
    if (htim == &htim3) { // 10kHz
        zumo().hcsr04.ISR();
        zumo().encoderR.encoder10kHzTickISR();
        zumo().encoderL.encoder10kHzTickISR();

        static uint16_t divider_10Hz = 0;
        static uint16_t divider_100Hz = 0;

        if (++divider_100Hz == 100 ) {
            divider_100Hz = 0;

//            if (driver_enable) {
//                zumo().motor_driver.set_differential(0.3, zumo().line_detector.get_line_position()*0.25 );
//            } else {
//                zumo().motor_driver.Motor_A.set_duty_cycle(0);
//                zumo().motor_driver.Motor_B.set_duty_cycle(0);
//            }
        }

        if (++divider_10Hz == 1000) { // 10Hz
            divider_10Hz = 0;
            _10Hz_flag = true;
        }
    }
}