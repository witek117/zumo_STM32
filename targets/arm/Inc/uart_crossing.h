#ifndef ZUMO_UART_CROSSING_H
#define ZUMO_UART_CROSSING_H

#include <command_terminal/command_manager.h>
#include <window_terminal/VT100.hpp>
#include <window_terminal/window_manager.hpp>
#include "main.h"
#include "stm32f1xx_hal_conf.h"

extern CommandManager <11,'\r', false>command_manager;

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
        uint32_t isrflags   = READ_REG(huart.Instance->SR);
        uint32_t cr1its     = READ_REG(huart.Instance->CR1);
        uint8_t data = huart.Instance->DR;
        uint32_t errorflags = 0x00U;

        /* If no error occurs */
        errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
        if (errorflags == RESET) {
            /* UART in mode Receiver -------------------------------------------------*/
            if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)) {
                on_receive(static_cast<char >(data));
                return 1;
            }
        }

        if (isrflags & USART_SR_IDLE) {
            huart.Instance->SR &= ~ USART_SR_IDLE;
        }

        if (isrflags & USART_SR_RXNE) {
            huart.Instance->SR &= ~ USART_SR_RXNE;
        }

//        if (isrflags & USART_SR_ORE) {
//            volatile uint8_t data = huart.Instance->DR;
//            (void) data;
//            huart.Instance->SR &= ~ USART_SR_ORE;
//        }

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
