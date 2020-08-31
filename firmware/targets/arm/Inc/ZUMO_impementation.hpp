# pragma once

#include "MPU6050.hpp"
#include "main.h"
#include "STM_hal.h"
#include "DRV8833.hpp"
#include "encoder.hpp"
#include "UART.hpp"
#include "STM_WS2812B.hpp"
#include "BME280.hpp"
#include "MCP9700.hpp"
#include "command_terminal/command_manager.h"

class ZUMO {
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

    // MCP9700
    static MCP9700<uint16_t > mcp9700; //((uint16_t&)(*TEMP), 4095, 3.3f);
    static void set_mcp9700_enable_callback(const char* data);
    static void get_mcp9700_value_callback(const char* data);

    // MOTORS
    static STM32_PWM<1000> PWM_1;
    static STM32_PWM<1000> PWM_2;
    static STM32_PWM<1000> PWM_3;
    static STM32_PWM<1000> PWM_4;

    static STM32_GPIO fault;
    static STM32_GPIO_FAKE nSleep;

    static DRV8833 motor_driver;

    static void set_motors_callback(const char* data);

    // ENCODERS
    static STM32_GPIO MOT_L_A;
    static STM32_GPIO MOT_L_B;
    static STM32_GPIO MOT_R_A;
    static STM32_GPIO MOT_R_B;

    static Encoder encoderL;
    static Encoder encoderR;

    static inline int16_t encoderLeftValue = 0;
    static inline int16_t encoderRightValue = 0;

    static void get_encoders_callback(const char* );

    // get actual current
    static void get_current_callback(const char*);

    // get battery value
    static void get_battery_value_callback(const char*);

    // UARTS
    static Uart uart1;

    // COMMAND MANAGER
    using CommandManagerTempalte = CommandManager <14>;
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

//    inline static bool printFlag = false;

    static void ISR_10kHz() {
        encoderL.encoder10kHzTickISR();
        encoderR.encoder10kHzTickISR();

        static uint16_t index = 0;

        index ++;
        if (index > 500) {
            index = 0;
            encoderLeftValue = encoderL.encoderGetCountAndReset();
            encoderRightValue = encoderR.encoderGetCountAndReset();
        }
//
//        static uint16_t indexPrint = 0;
//
//        indexPrint++;
//        if (indexPrint > 10000) {
//            indexPrint = 0;
//            printFlag = true;
//        }
    }

    static ZUMO& zumo() {
        static ZUMO staticZumo;
        return staticZumo;
    }

    static void loop();


// I2C
static STM32_I2C IMU_I2C;
};