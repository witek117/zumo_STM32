#pragma once

#include "main.hpp"
#include "STM32F301/UART.hpp"

//Comman_manager_template* command_manager;
Uart* uarts[3];

void uart1_handler(uint8_t c) {
    if (c == 'c') {
        //uarts[0]->setRedirectHandler(command_manager->print);
    }
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

//    void addCommandManager(Comman_manager_template* command_manager_) {
//        command_manager = command_manager_;
//    }

    void init() {
        for(uint8_t i = 0; i <  uart_counter; i++) {
             uarts[i]->init();
             uarts[i]->setRedirectHandler(uart1_handler);
        }
    }
};
