#ifndef ZUMO_HAL_H
#define ZUMO_HAL_H

#include "GPIO/GPIO.h"
#include "main.h"

class STM32_GPIO : public GPIO {
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

    void toggle() {
        HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    }

    bool get() override {
        return static_cast<bool>(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin));
    }
};


//class STM32_PWM : public PWM {
//    TIM_HandleTypeDef *htim;
//    uint32_t Channel;
//    uint32_t CCRx;
//
//public:
//    STM32_PWM(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t CCRx) : htim(htim), Channel(Channel), CCRx(CCRx) {
//
//    }
//
//    void set_duty_cycle(float) {
//        htim->Channel->Channel =
//    }
//
//    void off() {
//
//    }
//
//    void on() {
//
//    }
//
//
//
//
//};

#endif //ZUMO_HAL_H
