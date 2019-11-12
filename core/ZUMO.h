#ifndef ZUMO_ZUMO_H
#define ZUMO_ZUMO_H

#include "DRV8833.h"
#include "encoder.h"
#include "KTIR0711S.h"
#include "hal.h"

struct ZUMO {
    DRV8833& motor_driver;
    Encoder& encoderL;
    Encoder& encoderR;
    LineSensors<volatile uint16_t , 4095, 8> &line_sensors;
    hal::GPIO& LED1;
    hal::GPIO& LED2;

    ZUMO(DRV8833& motor_driver, Encoder& encoderL, Encoder& encoderR, LineSensors<volatile uint16_t , 4095, 8> &line_sensors, hal::GPIO& LED1, hal::GPIO& LED2)
        :   motor_driver(motor_driver),
            encoderL(encoderL),
            encoderR(encoderR),
            line_sensors(line_sensors),
            LED1(LED1),
            LED2(LED2){ }
};

ZUMO& zumo(void);

#endif //ZUMO_ZUMO_H
