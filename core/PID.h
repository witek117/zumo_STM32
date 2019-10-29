#ifndef ZUMO_PID_H
#define ZUMO_PID_H

#include <cmath>
#include <algorithm>
#include <array>

class PID {
public:
    float Kp, Ki, Kd;
    uint16_t max_power;

    float target;
    float lowerLimit, upperLimit;
    float error;
    float prevError;
    float integral;

    constexpr PID(float Kp, float Ki, float Kd, uint16_t max_power, float target, float lowerLimit, float upperLimit) noexcept :
        Kp(Kp), Ki(Ki), Kd(Kd), max_power(max_power),
        target(target), lowerLimit(lowerLimit), upperLimit(upperLimit),
        error{0}, prevError{0}, integral{0} {
        this->reset();
    }

    float tick(float data) {
        float actualUpperLimit = std::min(upperLimit, static_cast<float>(max_power));

        error = target - data;

        float diff = error - prevError;
        float pwr = Kp * error + Ki * (integral + error) + Kd * diff;

        if (lowerLimit < pwr && pwr < actualUpperLimit) {
            integral += error;
        }

        prevError = error;

        return std::clamp(pwr, lowerLimit, actualUpperLimit);
    }

    void reset() {
        this->integral = 0;
        this->prevError = 0;
    }
};

#endif //ZUMO_PID_H
