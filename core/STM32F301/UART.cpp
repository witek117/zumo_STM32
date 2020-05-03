#include "UART.hpp"
#include <stm32f3xx.h>
#include <ZUMO_devices/ZUMO.hpp>
#include "stm32f3xx_hal_rcc.h"

#define UART_BRR_MIN    0x10U        /* UART BRR minimum authorized value */
#define UART_BRR_MAX    0x0000FFFFU  /* UART BRR maximum authorized value */

Uart* uartHandlers[3];

Uart::Uart(USART_TypeDef* uart, uint32_t baudrate) :
        uart(uart), baudrate(baudrate), redirectHandler(nullptr) {
    if(USART1 == uart){
        uartHandlers[0] = this;
    } else if(USART2 == uart){
        uartHandlers[1] = this;
    } else if(USART3 == uart){
        uartHandlers[2] = this;
    }
    RingBuffer_Init(&rxRingBuffer, rxBuffer, rxBufferSize);
    RingBuffer_Init(&txRingBuffer, txBuffer, txBufferSize);
}

void Uart::init(){
    // disable
    uart->CR1 &= ~ USART_CR1_UE;
    if(uart == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        RCC->CFGR3 &= ~(RCC_CFGR3_USART1SW_Msk);
        RCC->CFGR3 |= RCC_CFGR3_USART1SW_PCLK1;
    } else if(uart == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    } else if(uart == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    } else {
        return;
    }

    /* In multiprocessor mode, the following bits must be kept cleared:
    - LINEN and CLKEN bits in the USART_CR2 register,
    - SCEN, HDSEL and IREN  bits in the USART_CR3 register. */
    CLEAR_BIT(uart->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
    CLEAR_BIT(uart->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

    uint32_t usartdiv = 0x00000000U;
    uint32_t pclk;

    pclk = HAL_RCC_GetPCLK1Freq();

    usartdiv = (uint16_t)((((pclk)) + ((baudrate)/2U)) / (baudrate));

    if ((usartdiv >= UART_BRR_MIN) && (usartdiv <= UART_BRR_MAX)) {
        uart->BRR = usartdiv;
    }

    // enable
    uart->CR1 |= USART_CR1_UE;

    // transmit and receive
    uart->CR1 |= USART_CR1_TE | USART_CR1_RE;

    if(USART1 == uart){
        NVIC_ClearPendingIRQ(USART1_IRQn);
        NVIC_EnableIRQ(USART1_IRQn);
    } else if(USART2 == uart){
        NVIC_ClearPendingIRQ(USART2_IRQn);
        NVIC_EnableIRQ(USART2_IRQn);
    } else if(USART3 == uart){
        NVIC_ClearPendingIRQ(USART3_IRQn);
        NVIC_EnableIRQ(USART3_IRQn);
    }
    enableInterrupt(InterruptType::RX_FULL);
}

void Uart::write(void const* data, uint16_t length){
    // enter critical section
//    __disable_irq();
    // put data to ring buffer
    for(uint16_t i=0; i<length; i++){
        RingBuffer_PutChar(&txRingBuffer, *(reinterpret_cast<char*>(const_cast<void*>(data)) + i));
    }
    // exit critical section
//    __enable_irq();
//    if (!DMAworking) {
        // enable TX empty interrupt
        enableInterrupt(InterruptType::TX_EMPTY);
//    }

}

void Uart::write(uint8_t data) {
    // enter critical section
//    __disable_irq();
    // put data to ring buffer
    RingBuffer_PutChar(&txRingBuffer, data);
    // exit critical section
//    __enable_irq();
//    if (!DMAworking) {
        // enable TX empty interrupt
        enableInterrupt(InterruptType::TX_EMPTY);
//    }
}

uint8_t Uart::read() {
    static uint8_t c;
    RingBuffer_GetChar(&rxRingBuffer, &c);
    return c;
}

void Uart::read(uint8_t* buffer, uint16_t length){
    if(buffer != nullptr){
        if(length <= RingBuffer_GetLen(&rxRingBuffer)){
            for(auto i=0; i<length; i++){
                RingBuffer_GetChar(&rxRingBuffer, &buffer[i]);
            }
        }
    }
}

bool Uart::isBufferEmpty(){
    return RingBuffer_IsEmpty(&rxRingBuffer);
}

uint16_t Uart::getBufferLevel(){
    return RingBuffer_GetLen(&rxRingBuffer);
}

void Uart::flushRxBuffer(){
    RingBuffer_Clear(&rxRingBuffer);
}

void Uart::enableInterrupt(InterruptType interrupt){
    (void) interrupt;
    uart->CR1 |= static_cast<uint8_t >(interrupt);
}

void Uart::disableInterrupt(InterruptType interrupt){
    (void) interrupt;
    uart->CR1 &= ~(static_cast<uint8_t >(interrupt));
}

void UART_IRQ(Uart* uartHandler) {
    uint32_t isrflags   = READ_REG(uartHandler->uart->ISR);
    uint32_t cr1its     = READ_REG(uartHandler->uart->CR1);

    if(isrflags & USART_ISR_TXE) {
        uint8_t c;
        if (RingBuffer_GetChar(&(uartHandler->txRingBuffer), &c)) {
            uartHandler->uart->TDR = c;
        } else {
            uartHandler->uart->RQR |= USART_RQR_TXFRQ;
            uartHandler->disableInterrupt(Uart::InterruptType::TX_EMPTY);
            uartHandler->disableInterrupt(Uart::InterruptType::TX_COMPLETE);
        }
    }

    if (isrflags & USART_ISR_RXNE) {
        uint8_t data        = uartHandler->uart->RDR;

        /* If no error occurs */
        uint32_t errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
        if (errorflags == 0U)
        {
            /* UART in mode Receiver ---------------------------------------------------*/
            if (((isrflags & USART_ISR_RXNE) != 0U)
                && ((cr1its & USART_CR1_RXNEIE) != 0U))
            {
                if (uartHandler->redirectHandler) {
                    uartHandler->redirectHandler(data);
                } else {
                    RingBuffer_PutChar(&(uartHandler->rxRingBuffer), data);
                }
            }
        }
    }

    if (isrflags & USART_ISR_IDLE) {
        uartHandler->uart->ICR |=  USART_ICR_IDLECF;
    }

    if (isrflags & USART_ISR_TC) {
        uartHandler->uart->ICR |= USART_ICR_TCCF;
    }

    if (isrflags & USART_ISR_TXE) {
        uartHandler->uart->RQR |= USART_RQR_TXFRQ;
    }
}

extern "C" {
void USART1_IRQHandler() {
    UART_IRQ(uartHandlers[0]);
}

void USART2_IRQHandler() {
    UART_IRQ(uartHandlers[1]);
}

void USART3_IRQHandler() {
    UART_IRQ(uartHandlers[2]);
}
}