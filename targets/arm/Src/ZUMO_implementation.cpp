#include "ZUMO_impementation.hpp"
#include "STM_hal.h"

// ZUMO LEDS
STM32_GPIO ZUMO::LED1 = {LED1_GPIO_Port, LED1_Pin};

STM32_GPIO ZUMO::LED2 = {LED2_GPIO_Port, LED2_Pin};

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
        Command("test", ZUMO::test_callback)
    });

void ZUMO::test_callback(const char* data) {
    (void) data;
    command_manager.printer.print("1234");
}

void printUart1(uint8_t c) {
    ZUMO::uart1.write(c);
}

void ZUMO::init() {
    motor_driver.init();
    uart1.init();
    command_manager.init();
    PrintManager::setPrintFunction(printUart1);
    command_manager.printer.print("siema");
    uart1.setRedirectHandler(ReadManager::putChar);

}