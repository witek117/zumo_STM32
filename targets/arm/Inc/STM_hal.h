#ifndef ZUMO_STM_HAL_H
#define ZUMO_STM_HAL_H

#include "hal.h"
#include "main.h"

class STM32_GPIO_FAKE : public hal::GPIO {
public:
    STM32_GPIO_FAKE () = default ;
    void set() override { }
    void reset() override { }
    void toggle() override { }
    bool get() override { return false; }
};

class STM32_GPIO : public hal::GPIO {
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
public:
    STM32_GPIO (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) : GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) {

    }

    void set() override {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
    }

    void reset() override {
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
    }

    void toggle() override {
        HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    }

    bool get() override {
        return static_cast<bool>(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin));
    }
};

template <uint32_t max_value>
class STM32_PWM : public hal::PWM {
    volatile uint32_t &CCRx;
public:
    STM32_PWM(volatile uint32_t &CCRx) : CCRx(CCRx) { }

    void set_duty_cycle(float duty_cycle) override {
        if (duty_cycle < 0) {
            CCRx = 0;
            return;
        } else if (duty_cycle >= 1) {
            CCRx = max_value;
            return;
        }
        duty_cycle *= 1000;
        CCRx = static_cast<uint32_t>(duty_cycle);
    }

    void off() override {
        CCRx = 0;
    }

    void on() override {
        CCRx = max_value;
    }

    float get_duty_cycle() override {
        return static_cast<float>(CCRx) / 1000.0f;
    }
};

class STC32_UART {

};


#endif //ZUMO_STM_HAL_H
