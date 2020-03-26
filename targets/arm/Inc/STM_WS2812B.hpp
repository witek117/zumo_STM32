#ifndef ZUMO_STM_WS2812B_H
#define ZUMO_STM_WS2812B_H

#include "main.h"
#include <functional>
#include <utility>
#include "../../../core/ZUMO_devices/WS2812B.hpp"

#define WS_Pin WS2812B_Pin
#define WS_Port GPIOC

void ws2812b_send(uint32_t data) {
    static uint32_t pin_off = (uint32_t)WS_Pin << 16u;
    static uint32_t pin_on = WS_Pin;
    for (int8_t i = 23; i >= 0; i--) {
        WS_Port->BSRR = pin_on;
        if ((data & (1u << i)) == 0) {
            WS_Port->BSRR = pin_off;
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
        } else {
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop"); __asm("nop");
            WS_Port->BSRR = pin_off;
        }
    }
}

template <int size>
class STM_WS2812B : public WS2812B<size> {
    uint32_t data_buffer[size];
    std::function<void(void)> enable_interrupts;
    std::function<void(void)> disable_interrupts;
public:
    STM_WS2812B (std::function<void(void)> enable_interrupts, std::function<void(void)> disable_interrupts ) :
    data_buffer{0}, enable_interrupts(std::move(enable_interrupts)), disable_interrupts(std::move(disable_interrupts))
    { }

    void set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
        if (index <= size) {
            data_buffer[index] = uint32_t(red << 16u | green << 8u | blue);
        }
    }

    void send() {
        HAL_GPIO_WritePin(WS_Port, WS_Pin, GPIO_PIN_RESET);
        for (volatile uint32_t i =0; i < 3000; i ++) {}
        disable_interrupts();
        for (uint8_t i = 0; i < size; i++) {
            ws2812b_send(data_buffer[i]);
        }
        enable_interrupts();
        HAL_GPIO_WritePin(WS_Port, WS_Pin, GPIO_PIN_SET);
    }
};


#endif //ZUMO_STM_WS2812B_H
