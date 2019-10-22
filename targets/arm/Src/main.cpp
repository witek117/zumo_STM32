#include "main.h"
#include "hal.h"
#include "window_terminal/window_manager.hpp"
#include "vector"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart1;
uint8_t Received;
void usart_put_char(char ch) { HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10); }
volatile uint32_t localUartCounter;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance ) {

    }
    window_manager::in_RX_callback(Received);
    HAL_UART_Receive_DMA(&huart1, &Received, 1);
    localUartCounter = 2000;
}

std::vector<Window> windows;

extern "C"
void Main() {
    // LED configuration
    STM32_GPIO LED1(LED1_GPIO_Port, LED1_Pin);
    STM32_GPIO LED2(LED2_GPIO_Port, LED2_Pin);

    // PWM configuration
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

    STM32_PWM<1000> PWM_1_CH1(htim1.Instance->CCR1);
    STM32_PWM<1000> PWM_1_CH2(htim1.Instance->CCR2);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    STM32_PWM<1000> PWM_2_CH3(htim2.Instance->CCR3);
    STM32_PWM<1000> PWM_2_CH4(htim2.Instance->CCR4);

    // terminal configuration
    VT::init(usart_put_char);

    windows.emplace_back(Window("memsy",  1, 1, 10, 10, true));
    window_manager::init(&windows);
    window_manager::refresh_all();

    LED1.set();
    LED2.reset();
    
    PWM_1_CH1.set_duty_cycle(0.1);
    PWM_1_CH2.set_duty_cycle(0.4);

    while(1) {
        window_manager::run();
        LED1.toggle();
        LED2.toggle();
        HAL_Delay(100);
    }
}