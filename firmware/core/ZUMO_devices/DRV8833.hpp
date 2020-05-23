#pragma once

#include <cstdint>
#include "ZUMO_hal/HALina_PWM.hpp"
#include "ZUMO_hal/HALina_GPIO.hpp"

class DRV8833 {
public:
    class MotorChannel {
    public:
        enum class RunningMode : uint8_t {
            STOPPED,
            RUNNING
        };

        enum class Mode : uint8_t {
            FORWARD_FAST_DECAY,
            FORWARD_SLOW_DECAY,
            REVERSE_FAST_DECAY,
            REVERSE_SLOW_DECAY
        };
    private:
        PWM &IN1;
        PWM &IN2;

        float last_IN1_duty_cycle = 0;
        float last_IN2_duty_cycle = 0;

        Mode mode = Mode::FORWARD_FAST_DECAY;
        RunningMode runningMode = RunningMode::RUNNING;
    public:
        MotorChannel(PWM &IN1, PWM &IN2): IN1(IN1), IN2(IN2) { }

        void init() {
            IN1.init();
            IN2.init();
        }

        void brake() {
            if (runningMode == RunningMode::STOPPED) {
                return;
            }
            last_IN1_duty_cycle = IN1.get_duty_cycle();
            last_IN2_duty_cycle = IN2.get_duty_cycle();

            IN1.on();
            IN2.on();
            runningMode = RunningMode::STOPPED;
        }

        void run() {
            if (runningMode == RunningMode::RUNNING) {
                return;
            }
            IN1.set_duty_cycle(last_IN1_duty_cycle);
            IN2.set_duty_cycle(last_IN2_duty_cycle);
            runningMode = RunningMode::RUNNING;
        }

        RunningMode get_running_mode() {
            return runningMode;
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

private:
    HALina_GPIO &Sleep;
    HALina_GPIO &FAULT;
public:
    MotorChannel Motor_A;
    MotorChannel Motor_B;

    enum class EnableMode : bool {
        ENABLE = true,
        DISABLE = false
    };

    enum class Status : uint8_t {
        FAULT,
        WORKING
    };

    DRV8833(PWM &AIN1, PWM &AIN2, PWM &BIN1, PWM &BIN2, HALina_GPIO &Sleep, HALina_GPIO &FAULT): Sleep(Sleep), FAULT(FAULT), Motor_A(AIN1, AIN2), Motor_B(BIN1, BIN2) {
        set_enable(EnableMode::DISABLE);
    }

    void init() {
        Motor_A.init();
        Motor_B.init();
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

    void set_differential(float duty_cycle, float difference) {
        Motor_A.set_duty_cycle(duty_cycle - difference);
        Motor_B.set_duty_cycle(duty_cycle + difference );
    }
};