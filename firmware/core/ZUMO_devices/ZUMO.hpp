#pragma once

#include "DRV8833.hpp"
#include "encoder.hpp"
#include "KTIR0711S.hpp"
#include "HC-SR04.hpp"
#include "MCP9700.hpp"
#include "BME280.hpp"
#include "MPU6050.hpp"
#include "WS2812B.hpp"

//struct ZUMO {
//    DRV8833& motor_driver;
//    Encoder& encoderL;
//    Encoder& encoderR;
//    LineSensors<volatile uint16_t , 4095, 8> &line_sensors;
//    HALina_GPIO& LED1;
//    HALina_GPIO& LED2;
//    HCSR04& hcsr04;
//    MCP9700<uint16_t >& mcp9700;
//    BME280& bme280;
//    MPU6050& mpu6050;
//    WS2812B<2>& ws2812b;
//
//    ZUMO(DRV8833& motor_driver, Encoder& encoderL, Encoder& encoderR, LineSensors<volatile uint16_t , 4095, 8> &line_sensors,
//         HALina_GPIO& LED1, HALina_GPIO& LED2, HCSR04& hcsr04, MCP9700<uint16_t >& mcp9700, BME280& bme280, MPU6050& mpu6050, WS2812B<2>& ws2812b)
//        :   motor_driver(motor_driver),
//            encoderL(encoderL),
//            encoderR(encoderR),
//            line_sensors(line_sensors),
//            LED1(LED1),
//            LED2(LED2),
//            hcsr04(hcsr04),
//            mcp9700(mcp9700),
//            bme280(bme280),
//            mpu6050(mpu6050),
//            ws2812b(ws2812b) {
//    }
//
//    void init() {
//        motor_driver.init();
//        motor_driver.Motor_A.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
//        motor_driver.Motor_B.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
//        motor_driver.Motor_A.set_duty_cycle(0.7);
//        motor_driver.Motor_B.set_duty_cycle(0.7);
//        motor_driver.Motor_A.brake();
//        motor_driver.Motor_B.brake();
//        LED2.toggle();
//        mcp9700.init();
//        bme280.init();
//        mpu6050.init(MPU6050::GyroscopeData::Scale::DPS_2000, MPU6050::AccelerometerData::Range::G2);
//        mpu6050.gyroscope.calibrate(5);
//    }
//
//    void loop() {
//
//    }
//
//};
//
//ZUMO& zumo(void);

