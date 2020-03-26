#include "GPIO.hpp"

#define GPIO_MODE             (0x00000003U)
#define EXTI_MODE             (0x10000000U)
#define GPIO_MODE_IT          (0x00010000U)
#define GPIO_MODE_EVT         (0x00020000U)
#define RISING_EDGE           (0x00100000U)
#define FALLING_EDGE          (0x00200000U)
#define GPIO_OUTPUT_TYPE      (0x00000010U)

#define GPIO_NUMBER           (16U)

void GPIO::initAlternate() {
    //uint32_t position = pin;
    uint32_t temp;

    if((mode == Mode::AF_PP) || (mode == Mode::AF_OD)) {
        /* Configure Alternate function mapped with the current IO */
        temp = base->AFR[position >> 3u];
        temp &= ~(0xFu << ((position & 0x07u) * 4u));
        temp |= ((alternate) << ((position & 0x07u) * 4u));
        base->AFR[position >> 3u] = temp;
    }

/* Configure IO Direction mode (Input, Output, Alternate or Analog) */
    temp = base->MODER;
    temp &= ~(GPIO_MODER_MODER0 << (position * 2u));
    temp |= ((static_cast<uint32_t>(mode) & GPIO_MODE) << (position * 2u));
    base->MODER = temp;

/* In case of Output or Alternate function mode selection */
    if((mode == Mode::OUTPUT_PP) || (mode == Mode::AF_PP) ||
       (mode == Mode::OUTPUT_OD) || (mode == Mode::AF_OD)) {
        /* Check the Speed parameter */
        // assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
        /* Configure the IO Speed */
        temp = base->OSPEEDR;
        temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2u));
        temp |= (static_cast<uint32_t>(speed) << (position * 2u));
        base->OSPEEDR = temp;

        /* Configure the IO Output Type */
        temp = base->OTYPER;
        temp &= ~(GPIO_OTYPER_OT_0 << position) ;
        temp |= (((static_cast<uint32_t>(mode) & GPIO_OUTPUT_TYPE) >> 4u) << position);
        base->OTYPER = temp;
    }

    /* Activate the Pull-up or Pull down resistor for the current IO */
    temp = base->PUPDR;
    temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2u));
    temp |= ((pull) << (position * 2u));
    base->PUPDR = temp;
}


void GPIO::initEXTI() {
    //uint32_t position = pin;
    uint32_t iocurrent = position;
    uint32_t temp;

    iocurrent = 1u << position;

    if((static_cast<uint32_t>(mode) & EXTI_MODE) == EXTI_MODE) {
        /* Enable SYSCFG Clock */
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        temp = SYSCFG->EXTICR[position >> 2u];
        temp &= ~(0x0FuL << (4u * (position & 0x03u)));
        temp |= (baseIndex << (4u * (position & 0x03u)));
        SYSCFG->EXTICR[position >> 2u] = temp;

        /* Clear EXTI line configuration */
        temp = EXTI->IMR;
        temp &= ~(iocurrent);
        if((static_cast<uint32_t>(mode) & GPIO_MODE_IT) == GPIO_MODE_IT) {
            temp |= iocurrent;
        }
        EXTI->IMR = temp;

        temp = EXTI->EMR;
        temp &= ~(iocurrent);
        if((static_cast<uint32_t>(mode) & GPIO_MODE_EVT) == GPIO_MODE_EVT) {
            temp |= iocurrent;
        }
        EXTI->EMR = temp;

        /* Clear Rising Falling edge configuration */
        temp = EXTI->RTSR;
        temp &= ~(iocurrent);
        if((static_cast<uint32_t>(mode) & RISING_EDGE) == RISING_EDGE) {
            temp |= iocurrent;
        }
        EXTI->RTSR = temp;

        temp = EXTI->FTSR;
        temp &= ~(iocurrent);
        if((static_cast<uint32_t>(mode) & FALLING_EDGE) == FALLING_EDGE) {
            temp |= iocurrent;
        }
        EXTI->FTSR = temp;
    }

}
