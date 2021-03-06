#pragma once

#include "commands_interface.h"

class HCSR04 : CommandsInterface {
    HALina_GPIO& trig;
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
    HCSR04(HALina_GPIO& trig) : trig(trig), counter(0), counted(0) { }

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
        if (enable) {
            state = State::STARTED;
            counter = 0;
        }

    }

    uint16_t get_last_value() {
        return counted;
    }

    void init() {
//        enable = true;
    }

    void run_measurements() {
        start();
    }

    void set_enable(bool enable_) override {
        this->enable = enable_;
        if (!enable_) {
            counted = 0;
        }
    }
};
