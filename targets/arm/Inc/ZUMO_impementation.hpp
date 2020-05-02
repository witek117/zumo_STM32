# pragma once

#include "main.h"
#include "STM_hal.h"
#include "DRV8833.hpp"
#include "encoder.hpp"
#include "UART.hpp"
#include "command_terminal/command_manager.h"

class ZUMO {

    // MOTORS
    static STM32_PWM<1000> PWM_1;
    static STM32_PWM<1000> PWM_2;
    static STM32_PWM<1000> PWM_3;
    static STM32_PWM<1000> PWM_4;

    static STM32_GPIO fault;
    static STM32_GPIO_FAKE nSleep;

    static DRV8833 motor_driver;

    // ENCODERS
    static STM32_GPIO MOT_L_A;
    static STM32_GPIO MOT_L_B;
    static STM32_GPIO MOT_R_A;
    static STM32_GPIO MOT_R_B;

    static Encoder encoderL;
    static Encoder encoderR;

    // UARTS
    static Uart uart1;

    // COMMAND MANAGER
    static CommandManager <1, '\n', true> command_manager;
    static void test_callback(const char*);

    static void enableInterrupts() {
        __enable_irq();
    }

    static void disableInterrupts() {
        __disable_irq();
    }


public:
    ZUMO() = default;

    void init() {
        motor_driver.init();
        uart1.init();
        uart1.write("siema", 5);
    }

    static void ISR_10kHz() {
        encoderL.encoder10kHzTickISR();
        encoderR.encoder10kHzTickISR();
    }

    static ZUMO& zumo() {
        static ZUMO staticZumo;
        return staticZumo;
    }

    static void loop() {
        command_manager.run();
    }



};