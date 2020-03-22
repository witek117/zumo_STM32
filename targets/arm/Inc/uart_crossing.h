#ifndef ZUMO_UART_CROSSING_H
#define ZUMO_UART_CROSSING_H

#include "command_terminal/command_manager.h"
#include "external/window_terminal/VT100.hpp"
#include "external/window_terminal/window_manager.hpp"
#include "main.h"
#include "stm32f3xx_hal_conf.h"

extern CommandManager <16,'\r', false>command_manager;

class STM32_UART {
public:
    enum class UARTMode {
        TERMINAL,
        COMMAND,
        NONE
    };
    UARTMode uart_mode;
private:
    UART_HandleTypeDef &huart;
    std::function<void(char)> fun;
public:
    STM32_UART(UART_HandleTypeDef &huart): uart_mode(UARTMode::NONE), huart(huart), fun(nullptr) {}

    void send(char ch) {

        HAL_UART_Transmit(&huart, reinterpret_cast<uint8_t *>(&ch), 1, 10);
    }

    int interrupt() {
        uint32_t isrflags   = READ_REG(huart.Instance->ISR);
        uint32_t cr1its     = READ_REG(huart.Instance->CR1);

        uint8_t data        = huart.Instance->RDR;

        /* If no error occurs */
        uint32_t errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
        if (errorflags == 0U)
        {
            /* UART in mode Receiver ---------------------------------------------------*/
            if (((isrflags & USART_ISR_RXNE) != 0U)
                && ((cr1its & USART_CR1_RXNEIE) != 0U))
            {
                on_receive(static_cast<char >(data));
                return 1;
            }
        }

        if (isrflags & USART_ISR_IDLE) {
            huart.Instance->ISR &= ~ USART_ISR_IDLE;
        }

        if (isrflags & USART_ISR_RXNE) {
            huart.Instance->ISR &= ~ USART_ISR_RXNE;
        }
        return 1;
    }

    void on_receive(char c) {
        if (uart_mode == UARTMode::NONE) {

            if (c == 't') {
                if (VT::init([this](char ch) {this->send(ch);})) {
                    fun = &window_manager::in_RX_callback;
                    VT::refresh();
                    window_manager::refresh_value(true);
                    window_manager::refresh_all();
                    uart_mode = UARTMode::TERMINAL;
                }
            } else if (c == 'c') {
                zumo().LED1.toggle();
                if (command_manager.init([this](char ch) {this->send(ch);})) {
                    fun = [](char ch) {command_manager.put_char(ch);};
                    uart_mode = UARTMode::COMMAND;
                }
            }
            return;
        } else {
            if (fun) {
                fun(c);
            }
        }
    }
};


#endif //ZUMO_UART_CROSSING_H
