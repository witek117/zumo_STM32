#ifndef ZUMO_HC_SR04_H
#define ZUMO_HC_SR04_H

#include "hal.h"

class HCSR04 {
    hal::GPIO& trig;
    uint16_t counter;
    uint16_t counted;
    bool enable = false;

    enum class State {
        OFF,
        STARTED,
        TRIGGER,
        WAIT_FOR_RISING_EDGE,
        WAIT_FOR_FALLING_EDGE
    };

    State state = State::OFF;
public:
    HCSR04(hal::GPIO& trig) : trig(trig), counter(0), counted(0) { }

    void ISR() {
        if (!enable) return;
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
        if (!enable) return;
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

    void init() {
        enable = true;
    }

    void deinit() {
        enable = false;
        counted = 0;
    }
};



#endif //ZUMO_HC_SR04_H
