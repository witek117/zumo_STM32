#ifndef ZUMO_DRV8833_H
#define ZUMO_DRV8833_H

#include "GPIO/GPIO.h"

class DRV8833 {
    class MotorChannel {
        enum class Mode : uint8_t {
            FORWARD_FAST_DECAY,
            FORWARD_SLOW_DECAY,
            REVERSE_FAST_DECAY,
            REVERSE_SLOW_DECAY
        };

        PWM &IN1;
        PWM &IN2;

        Mode mode = Mode::FORWARD_FAST_DECAY;

    public:
        MotorChannel(PWM &IN1, PWM &IN2): IN1(IN1), IN2(IN2) {

        }

        void brake() {
            IN1.on();
            IN2.on();
        }

        void set_mode(Mode mode_) {
            this->mode = mode_;
        }

        void set_duty_cycle(float duty_cycle) {
            if (mode == Mode::FORWARD_FAST_DECAY) {
                IN1.set_duty_cycle(duty_cycle);
                IN2.off();
            } else if (mode == Mode::FORWARD_SLOW_DECAY) {
                IN1.on();
                IN2.set_duty_cycle(duty_cycle);
            } else if (mode == Mode::REVERSE_FAST_DECAY) {
                IN1.off();
                IN2.set_duty_cycle(duty_cycle);
            } else if (mode == Mode::REVERSE_SLOW_DECAY) {
                IN1.set_duty_cycle(duty_cycle);
                IN2.on();
            }
        }
    };

public:
    MotorChannel Motor_A;
    MotorChannel Motor_B;

    GPIO &Sleep;
    GPIO &FAULT;

    enum class EnableMode : bool {
        ENABLE = true,
        DISABLE = false
    };

    enum class Status : uint8_t {
        FAULT,
        WORKING
    };

    DRV8833(PWM &AIN1, PWM &AIN2, PWM &BIN1, PWM &BIN2, GPIO &Sleep, GPIO &FAULT): Motor_A(AIN1, AIN2), Motor_B(BIN1, BIN2), Sleep(Sleep), FAULT(FAULT) {
        set_enable(EnableMode::DISABLE);
    }

    Status get_status() {
        if (!FAULT.get()) {
            return Status::FAULT;
        } else {
            return Status::WORKING;
        }
    }

    void set_enable(EnableMode mode) {
        if (mode == EnableMode::ENABLE) {
            Sleep.set();
        } else {
            Sleep.reset();
        }
    }
};


#endif //ZUMO_DRV8833_H
