#ifndef ZUMO_ZUMO_H
#define ZUMO_ZUMO_H

#include "DRV8833.h"
#include "encoder.h"
#include "KTIR0711S.h"

struct ZUMO {
    DRV8833& motor_driver;
    Encoder& encoderL;
    Encoder& encoderR;
    LineDetector<volatile uint16_t, 8> &line_detector;

    ZUMO(DRV8833& motor_driver, Encoder& encoderL, Encoder& encoderR, LineDetector<volatile uint16_t, 8> &line_detector)
        :   motor_driver(motor_driver),
            encoderL(encoderL),
            encoderR(encoderR),
            line_detector(line_detector) { }
};

ZUMO& zumo(void);

#endif //ZUMO_ZUMO_H
