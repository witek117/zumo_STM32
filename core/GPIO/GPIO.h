#ifndef ZUMO_GPIO_H
#define ZUMO_GPIO_H


#include <cstdint>

class GPIO {
public:

    GPIO(){

    }

    enum class Mode : uint8_t {
        INPUT,
        OUTPUT,
        TIMER,
    };

    virtual void set() = 0;
    virtual void reset() = 0;
    virtual bool get() = 0;


};

class PWM {
public:
    virtual void set_duty_cycle(float) = 0;
    virtual void off() = 0;
    virtual void on() = 0;
    virtual float get_duty_cycle() = 0;

    PWM() {

    }
};


#endif //ZUMO_GPIO_H
