# pragma once

#include "main.h"
#include "STM_hal.h"
#include "DRV8833.hpp"
#include "encoder.hpp"
#include "UART.hpp"
#include "STM_WS2812B.hpp"
#include "command_terminal/command_manager.h"

class ZUMO {
public:
    // LEDS
    static STM32_GPIO LED1; //(LED1_GPIO_Port, LED1_Pin);
    static STM32_GPIO LED2; //(LED2_GPIO_Port, LED2_Pin);

    // WS2812
    static STM_WS2812B<2> ws2812b;
    static void set_value_value_callback(const char*);

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
    using CommandManagerTempalte = CommandManager <2>;
    static CommandManagerTempalte command_manager;
    static void test_callback(const char*);

    static void enableInterrupts() {
        __enable_irq();
    }

    static void disableInterrupts() {
        __disable_irq();
    }

    ZUMO() = default;

    void init();

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

        static uint32_t k = 0;
        k++;
        if (k > 100000) {
            k =  0;
            LED1.toggle();
        }
    }



};