# pragma once

#include "MPU6050.hpp"
#include "main.h"
#include "STM_hal.h"
#include "DRV8833.hpp"
#include "encoder.hpp"
#include "UART.hpp"
#include "STM_WS2812B.hpp"
#include "BME280.hpp"
#include "command_terminal/command_manager.h"

class ZUMO {
    // I2C
    static STM32_I2C IMU_I2C;
public:
    // LEDS
    static STM32_GPIO LED1; //(LED1_GPIO_Port, LED1_Pin);
    static STM32_GPIO LED2; //(LED2_GPIO_Port, LED2_Pin);

    // WS2812
    static STM_WS2812B<2> ws2812b;
    static void set_value_value_callback(const char*);

    // BME280
    static BME280 bme280; //(IMU_I2C, 0b1110110);
    static void set_bme280_enable_callback(const char*);
    static void get_bme280_value_callback(const char*);

    // MPU6050
    static MPU6050 mpu6050;// (IMU_I2C, 0x68);
    static void get_mpu_gyroscope_value_callback(const char*);
    static void get_mpu_accelerometer_value_callback(const char*);
    static void set_mpu_accelerometer_enable_callback(const char*);
    static void set_mpu_gyroscope_enable_callback(const char*);

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
    using CommandManagerTempalte = CommandManager <8>;
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
        bme280.run_measurements();
        mpu6050.run_measurements();

        static uint32_t k = 0;
        k++;
        if (k > 100000) {
            k =  0;
            LED1.toggle();
        }
    }



};