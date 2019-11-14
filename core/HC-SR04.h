#ifndef ZUMO_HC_SR04_H
#define ZUMO_HC_SR04_H

#include "hal.h"

// 150us to 25ms
// 2cm   to 2m
class HCSR04 {
    hal::GPIO& trig;
//    uint16_t  counter_state_1;
//    uint16_t  counter_state_2;
    uint16_t counter;
    uint16_t counted;

    enum class State {
        OFF,
        STARTED,
        TRIGGER,
        WAIT_FOR_RISING_EDGE,
        WAIT_FOR_FALLING_EDGE
    };

    State state = State::OFF;
public:
    HCSR04(hal::GPIO& trig) : trig(trig), counter(0), counted(0)
//    counter_state_1(0), counter_state_2(0)
    {

    }

    void ISR() {
        if (state == State::STARTED) {
            trig.set();
            state = State::TRIGGER;
        } else if (state == State::TRIGGER) {
            trig.reset();
            state = State::WAIT_FOR_RISING_EDGE;
        } else if (state == State::WAIT_FOR_FALLING_EDGE) {
            counter++;
        }
    }

    void edge_detected() {
        if (state == State::WAIT_FOR_RISING_EDGE) {
            rising_edge_detected();
        } else if (state == State::WAIT_FOR_FALLING_EDGE) {
            falling_edge_detected();
        }
    }

    void rising_edge_detected() {
        state = State::WAIT_FOR_FALLING_EDGE;
    }

    void falling_edge_detected() {
        state = State::OFF;
        counted = counter;
    }

    void start() {
        state = State::STARTED;
        counter = 0;
    }

    uint16_t get_value() {
        return counted;
    }
};



#endif //ZUMO_HC_SR04_H
