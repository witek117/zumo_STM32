#pragma once

#include <stm32f301x8.h>
#include "cstdint"
#include "ring_buffer.h"

class Uart  {
public:
    USART_TypeDef* uart;
    RingBuffer rxRingBuffer{};
    RingBuffer txRingBuffer{};

private:
    uint32_t baudrate;

    constexpr static auto txBufferSize = 2048;
    constexpr static auto rxBufferSize = 1024;

    uint8_t txBuffer[txBufferSize] = {0};
    uint8_t rxBuffer[rxBufferSize] = {0};
public:
    void (*redirectHandler)(uint8_t);

    enum class InterruptType : uint8_t {
        TX_EMPTY = USART_CR1_TXEIE ,
        TX_COMPLETE = USART_CR1_TCIE,
        RX_FULL = USART_CR1_RXNEIE
    };
public:
    void setRedirectHandler( void (*redirectHandler_)(uint8_t)) {
        this->redirectHandler = redirectHandler_;
    }
    void enableInterrupt(InterruptType interrupt);

    void disableInterrupt(InterruptType interrupt);

    Uart(USART_TypeDef* uart, uint32_t baudrate);

    void init();

    void write(void const* data, uint16_t length);
    void write(uint8_t data);
    uint8_t read() ;
    bool isBufferEmpty();
    uint16_t getBufferLevel();
    void flushRxBuffer();
    void read(uint8_t* buffer, uint16_t length);
};

