#pragma once

#include "main.hpp"
#include "STM32F301/UART.hpp"
#include "command_terminal/command_manager.h"
#include "ZUMO.hpp"
Uart* uarts[3];

void uart0_write(uint8_t c) {
    uarts[0]->write(c);
}

void uart1_write(uint8_t c) {
    uarts[1]->write(c);
}

void uart2_write(uint8_t c) {
    uarts[2]->write(c);
}

void checkRedirection(Uart * uart, char c) {
    if (c == 'c') {
        zumo().LED1.toggle();
        uart->setRedirectHandler(PrintManager::printManagerPutChar);
        if (uarts[0] == uart) {
            PrintManager::setPrintFunction(uart0_write);
        } else if (uarts[1] == uart) {
            PrintManager::setPrintFunction(uart1_write);
        } else if (uarts[2] == uart) {
            PrintManager::setPrintFunction(uart2_write);
        }
    }
}

void uart0_read(uint8_t c) {
    checkRedirection(uarts[0], c);
}

void uart1_read(uint8_t c) {
    checkRedirection(uarts[1], c);
}

void uart2_read(uint8_t c) {
    checkRedirection(uarts[2], c);
}

class UART_crossing {
    uint8_t uart_counter = 0;
public:
    void addUart(Uart* uart) {
        if (uart_counter >= 3) {
            return;
        }
        uarts[uart_counter++] = uart;
    }

    void init() {
        for(uint8_t i = 0; i <  uart_counter; i++) {
             uarts[i]->init();
        }

        if (uarts[0]) {
            uarts[0]->setRedirectHandler(uart0_read);
        }

        if (uarts[1]) {
            uarts[1]->setRedirectHandler(uart1_read);
        }

        if (uarts[2]) {
            uarts[2]->setRedirectHandler(uart2_read);
        }

    }
};
