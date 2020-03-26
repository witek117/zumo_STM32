#ifndef ZUMO_STM_HAL_H
#define ZUMO_STM_HAL_H

#include <HALina_GPIO.hpp>
#include <HALina_I2C.hpp>
#include "main.h"
//#include "ZUMO_hal/HALina_GPIO.hpp"
#include "../../../core/ZUMO_hal/HALina_PWM.hpp"
//#include "ZUMO_hal/HALina_I2C.hpp"

class STM32_GPIO_FAKE : public HALina_GPIO {
public:
    STM32_GPIO_FAKE () = default ;
    void set() override { }
    void reset() override { }
    void toggle() override { }
    bool get() override { return false; }
};

class STM32_GPIO : public HALina_GPIO {
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
public:
    STM32_GPIO (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) : GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) { }

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
class STM32_PWM : public PWM {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    HAL_StatusTypeDef(*init_handler)(TIM_HandleTypeDef *, uint32_t);
    volatile uint32_t *CCRx;
public:
    STM32_PWM(TIM_HandleTypeDef *htim, uint32_t channel, HAL_StatusTypeDef(*init_handler)(TIM_HandleTypeDef *, uint32_t)) : htim(htim), channel(channel), init_handler(init_handler) {

    }

    void init() override {
        switch (channel){
            case TIM_CHANNEL_1:
                CCRx = &htim->Instance->CCR1;
                break;
            case TIM_CHANNEL_2:
                CCRx = &htim->Instance->CCR2;
                break;
            case TIM_CHANNEL_3:
                CCRx = &htim->Instance->CCR3;
                break;
            case TIM_CHANNEL_4:
                CCRx = &htim->Instance->CCR4;
                break;
            case TIM_CHANNEL_5:
                CCRx = &htim->Instance->CCR5;
                break;
            case TIM_CHANNEL_6:
                CCRx = &htim->Instance->CCR6;
                break;
        }

        init_handler(htim, channel);
    }

    void set_duty_cycle(float duty_cycle) override {
        if (duty_cycle < 0) {
            *CCRx = 0;
            return;
        } else if (duty_cycle >= 1) {
            *CCRx = max_value;
            return;
        }
        duty_cycle *= 1000;
        *CCRx = static_cast<uint32_t>(duty_cycle);
    }

    void off() override {
        *CCRx = 0;
    }

    void on() override {
        *CCRx = max_value;
    }

    float get_duty_cycle() override {
        return static_cast<float>(*CCRx) / 1000.0f;
    }
};

class STM32_I2C : public HALina_I2C {
    I2C_HandleTypeDef& hi2c;
    uint8_t data_buffer [10] = {0};
public:
    explicit STM32_I2C(I2C_HandleTypeDef& hi2c) : hi2c(hi2c) {}

    void write(uint8_t address, uint8_t register_address) override {
        HAL_I2C_Master_Transmit((I2C_HandleTypeDef*)&hi2c, address, (uint8_t*)&register_address, 1, 10);
    }

    void write(uint8_t address, uint8_t register_address, uint8_t data) override {
        uint8_t buffer[2] = {uint8_t (register_address), data};
        HAL_I2C_Master_Transmit((I2C_HandleTypeDef*)&hi2c, address, buffer, 2, 10);
    }

    void write(uint8_t address, uint8_t register_address, uint16_t data) override {
        uint8_t buffer[3] = {uint8_t (register_address), uint8_t(data >> 8u), uint8_t(data)};
        HAL_I2C_Master_Transmit((I2C_HandleTypeDef*)&hi2c, address, buffer, 3, 10);
    }

    uint8_t read(uint8_t address, uint8_t register_address) override {
        HAL_I2C_Master_Transmit((I2C_HandleTypeDef*)&hi2c, address, (uint8_t*)&register_address, 1, 10);
        HAL_I2C_Master_Receive((I2C_HandleTypeDef*)&hi2c, address, data_buffer, 1, 10);
        return data_buffer[0];
    }

    uint8_t* read(uint8_t address, uint8_t register_address, size_t size) override {
        (void) size;
        HAL_I2C_Master_Transmit((I2C_HandleTypeDef*)&hi2c, address, (uint8_t*)&register_address, 1, 10);
        HAL_I2C_Master_Receive((I2C_HandleTypeDef*)&hi2c, address, data_buffer, size, 10);
        return data_buffer;
    }
};


#endif //ZUMO_STM_HAL_H
