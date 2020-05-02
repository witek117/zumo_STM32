//
// Created by witek on 30/04/2020.
//

#include "ZUMO_impementation.hpp"
#include "STM_hal.h"


// ZUMO MOTORS
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim15;

STM32_PWM<1000> ZUMO::PWM_1 = {&htim1, TIM_CHANNEL_2, HAL_TIMEx_PWMN_Start};
STM32_PWM<1000> ZUMO::PWM_2 = {&htim1, TIM_CHANNEL_3, HAL_TIMEx_PWMN_Start};
STM32_PWM<1000> ZUMO::PWM_3 = {&htim1, TIM_CHANNEL_1, HAL_TIMEx_PWMN_Start};
STM32_PWM<1000> ZUMO::PWM_4 = {&htim15, TIM_CHANNEL_1, HAL_TIM_PWM_Start};

STM32_GPIO_FAKE ZUMO::nSleep;
STM32_GPIO ZUMO::fault(FAULT_GPIO_Port, FAULT_Pin);

DRV8833 ZUMO::motor_driver = {ZUMO::PWM_1, ZUMO::PWM_2, ZUMO::PWM_3, ZUMO::PWM_4,ZUMO::nSleep, ZUMO::fault };


// ZUMO ENCODERS
STM32_GPIO ZUMO::MOT_L_A = {MOT_L_A_GPIO_Port, MOT_L_A_Pin};
STM32_GPIO ZUMO::MOT_L_B = {MOT_L_A_GPIO_Port, MOT_L_B_Pin};
STM32_GPIO ZUMO::MOT_R_A = {MOT_R_A_GPIO_Port, MOT_R_A_Pin};
STM32_GPIO ZUMO::MOT_R_B = {MOT_R_B_GPIO_Port, MOT_R_B_Pin};

Encoder ZUMO::encoderL = {MOT_L_A, MOT_L_B, 1};
Encoder ZUMO::encoderR = {MOT_R_A, MOT_R_B, 1};

// ZUMO UARTS
Uart ZUMO::uart1 = {USART1, 230400};

ZUMO::CommandManagerTempalte ZUMO::command_manager(enableInterrupts, disableInterrupts, {
//        Command("s?", get_line_value_callback),
//        Command("h?", get_hcsro4_value_callback),
//        Command("t?", get_mcp9700_value_callback),
//        Command("b?", get_bme280_value_callback),
//        Command("ma?", get_mpu_accelerometer_value_callback),
//        Command("mg?", get_mpu_gyroscope_value_callback),
//        Command("m", set_motors_callback),
//        Command("s", set_line_enable_callback),
//        Command("h", set_hcsr04_enable_callback),
//        Command("t", set_mcp9700_enable_callback),
//        Command("b", set_bme280_enable_callback),
//        Command("ma", set_mpu_accelerometer_enable_callback),
//        Command("mg", set_mpu_gyroscope_enable_callback),
//        Command("ws", set_ws2812B_value_callback),
//        Command("exit", exit_callback),
        Command("test", ZUMO::test_callback)
    });

void ZUMO::test_callback(const char* data) {
    (void) data;
}