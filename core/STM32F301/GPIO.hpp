#pragma once

#include <stm32f301x8.h>
#include <cstdint>
#include "ZUMO_hal/HALina_GPIO.hpp"

class STM_GPIO : HALina_GPIO {
public:
    enum class Mode : uint32_t {
        INPUT                       =(0x00000000U),   /*!< Input Floating Mode                   */
        OUTPUT_PP                   =(0x00000001U),   /*!< Output Push Pull Mode                 */
        OUTPUT_OD                   =(0x00000011U),   /*!< Output Open Drain Mode                */
        AF_PP                       =(0x00000002U),   /*!< Alternate Function Push Pull Mode     */
        AF_OD                       =(0x00000012U),   /*!< Alternate Function Open Drain Mode    */
        ANALOG                      =(0x00000003U),   /*!< Analog Mode  */
        IT_RISING                   =(0x10110000U),   /*!< External Interrupt Mode with Rising edge trigger detection          */
        IT_FALLING                  =(0x10210000U),   /*!< External Interrupt Mode with Falling edge trigger detection         */
        IT_RISING_FALLING           =(0x10310000U),   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */
        EVT_RISING                  =(0x10120000U),   /*!< External Event Mode with Rising edge trigger detection               */
        EVT_FALLING                 =(0x10220000U),   /*!< External Event Mode with Falling edge trigger detection              */
        EVT_RISING_FALLING          =(0x10320000U)   /*!< External Event Mode with Rising/Falling edge trigger detection       */
    };

    enum class Speed : uint32_t {
        LOW      =(0x00000000U), /*!< range up to 2 MHz, please refer to the product datasheet */
        MEDIUM   =(0x00000001U), /*!< range  4 MHz to 10 MHz, please refer to the product datasheet */
        HIGH     =(0x00000003U)  /*!< range 10 MHz to 50 MHz, please refer to the product datasheet */
    };
private:
    GPIO_TypeDef* base;
    uint32_t pin;
    Mode mode;
    uint8_t alternate;
    Speed speed;
    uint32_t pull;
    uint8_t baseIndex;
    uint8_t position;

public:
    STM_GPIO(GPIO_TypeDef* base, uint32_t position, Mode mode, uint8_t alternate, Speed speed, uint32_t pull):
            base(base),
            pin(1u << position),
            mode(mode),
            alternate(alternate),
            speed(speed),
            pull(pull),
            position(position) {
    }

    void initAlternate();
    void initEXTI();

    void init() {
        if (base == GPIOA) {
            RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
            baseIndex=0;
        } else if (base == GPIOB) {
            RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
            baseIndex=1;
        } else if (base == GPIOC) {
            RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
            baseIndex=2;
        } else if (base == GPIOD) {
            RCC->AHBENR |= RCC_AHBENR_GPIODEN;
            baseIndex=3;
        } else if (base == GPIOF) {
            RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
            baseIndex=5;
        }

        initAlternate();
        initEXTI();
    }

    inline void set() {
        base->BSRR = pin;
    }

    inline void reset() {
        base->BRR =  pin;
    }

    bool get() {
        return (base->IDR & pin) != 0;
    }

    void toggle() {
        if (get()) {
            reset();
        } else {
            set();
        }
    }
};
