#pragma once

#include <cstdint>

class PWM {
public:
    virtual void set_duty_cycle(float) = 0;
    virtual void off() = 0;
    virtual void on() = 0;
    virtual float get_duty_cycle() = 0;
    virtual void init() = 0;
};