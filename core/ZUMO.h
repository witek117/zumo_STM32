#ifndef ZUMO_ZUMO_H
#define ZUMO_ZUMO_H

#include "DRV8833.h"
#include "encoder.h"
#include "KTIR0711S.h"
#include "hal.h"
#include "HC-SR04.h"
#include "MCP9700.h"
#include "BME280.h"
#include "MPU6050.h"

struct ZUMO {
    DRV8833& motor_driver;
    Encoder& encoderL;
    Encoder& encoderR;
    LineSensors<volatile uint16_t , 4095, 8> &line_sensors;
    hal::GPIO& LED1;
    hal::GPIO& LED2;
    HCSR04& hcsr04;
    MCP9700<uint16_t >& mcp9700;
    BME280& bme280;
    MPU6050& mpu6050;

    ZUMO(DRV8833& motor_driver, Encoder& encoderL, Encoder& encoderR, LineSensors<volatile uint16_t , 4095, 8> &line_sensors,
         hal::GPIO& LED1, hal::GPIO& LED2, HCSR04& hcsr04, MCP9700<uint16_t >& mcp9700, BME280& bme280, MPU6050& mpu6050)
        :   motor_driver(motor_driver),
            encoderL(encoderL),
            encoderR(encoderR),
            line_sensors(line_sensors),
            LED1(LED1),
            LED2(LED2),
            hcsr04(hcsr04),
            mcp9700(mcp9700),
            bme280(bme280),
            mpu6050(mpu6050){ }
};

ZUMO& zumo(void);

#endif //ZUMO_ZUMO_H
