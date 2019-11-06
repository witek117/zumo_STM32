#include "main.h"
#include "hal.h"
#include "window_terminal/window_manager.hpp"
#include "vector"
#include "DRV8833.h"
#include "encoder.h"
#include "PID.h"
#include "KTIR0711S.h"
#include "ZUMO.h"
#include "mean.h"
#include "command_terminal/commands.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

STM32_GPIO LED1(LED1_GPIO_Port, LED1_Pin);

void hal::enable_interrupts() {
    __enable_irq();
}


void hal::disable_interrupts() {
    __disable_irq();
}

STM32_PWM<1000> PWM_1_CH1(TIM1->CCR1);
STM32_PWM<1000> PWM_1_CH2(TIM1->CCR2);

STM32_PWM<1000> PWM_2_CH3(TIM2->CCR3);
STM32_PWM<1000> PWM_2_CH4(TIM2->CCR4);

STM32_GPIO fault(FAULT_GPIO_Port, FAULT_Pin);
STM32_GPIO_FAKE nSleep;

STM32_GPIO MOT_L_A(MOT_L_A_GPIO_Port, MOT_L_A_Pin);
STM32_GPIO MOT_L_B(MOT_L_A_GPIO_Port, MOT_L_B_Pin);

STM32_GPIO MOT_R_A(MOT_R_A_GPIO_Port, MOT_R_A_Pin);
STM32_GPIO MOT_R_B(MOT_R_A_GPIO_Port, MOT_R_B_Pin);

STM32_GPIO SENS_IR_ENABLE(SENS_IR_LED_GPIO_Port, SENS_IR_LED_Pin);

volatile uint16_t sensors[8];
LineSensors <volatile uint16_t , 4095, 8>line_sensors(sensors, SENS_IR_ENABLE);

ZUMO& zumo (void) {
    static DRV8833 motor_driver(PWM_1_CH1, PWM_1_CH2, PWM_2_CH3, PWM_2_CH4, nSleep, fault);

    static Encoder encoderL(MOT_L_A, MOT_L_B, 1);

    static Encoder encoderR(MOT_R_A, MOT_R_B, 1);

    static LineDetector<volatile uint16_t, 8> line_detector(line_sensors);

    static ZUMO _zumo (motor_driver, encoderL, encoderR, line_detector);

    return _zumo;
}

class STM32_UART {
public:
    enum class UARTMode {
        TERMINAL,
        COMMAND,
        NONE
    };
    UARTMode uart_mode;
private:
    UART_HandleTypeDef &huart;
    std::function<void(char)> fun;
public:
    STM32_UART(UART_HandleTypeDef &huart): uart_mode(UARTMode::NONE), huart(huart), fun(nullptr) {}

    void send(char ch) {
        HAL_UART_Transmit(&huart, reinterpret_cast<uint8_t *>(&ch), 1, 10);
    }

    void interrupt() {
        uint32_t isrflags   = READ_REG(huart.Instance->SR);
        uint32_t cr1its     = READ_REG(huart.Instance->CR1);
        uint32_t errorflags = 0x00U;

        /* If no error occurs */
        errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
        if (errorflags == RESET) {
            /* UART in mode Receiver -------------------------------------------------*/
            if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)) {
                on_receive(static_cast<char >(huart.Instance->DR));
                return;
            }
        }
    }

    void on_receive(char c) {
        if (uart_mode==UARTMode::NONE) {
            if (c == 't') {
                if (VT::init([this](char ch) {this->send(ch);})) {
                    fun = &window_manager::in_RX_callback;
                }
            } else if (c == 'c' ) {
                if (commands::terminal().init([this](char ch) {this->send(ch);})) {
                    fun = &hal::receive_char_interrupt;
                }
            }
            return;
        } else {
            if (fun) {
                fun(c);
            }
        }
    }
};

STM32_UART UART1(huart1);
STM32_UART UART2(huart2);
STM32_UART UART3(huart3);

extern "C" void handle_usart1_interrupt() {
    UART1.interrupt();
}

extern "C" void handle_usart2_interrupt() {
    UART2.interrupt();
}

extern "C" void handle_usart3_interrupt() {
    UART3.interrupt();
}

PID pid_L(0.4, 0.1, 0.01, 1, 1, -1, 1);
PID pid_R(0.4, 0.1, 0.01, 1, 1, -1, 1);

float actual_line_position = 0.0;
volatile bool refresh_values = false;
volatile bool _10Hz_flag = false;
void _10Hz();

extern "C"
void Main() {
    zumo().line_detector.init();
    // LED configuration

    STM32_GPIO LED2(LED2_GPIO_Port, LED2_Pin);



    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

    // command_terminal configuration
//    VT::init(usart_put_char);







    LED1.set();
    LED2.reset();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensors, 8);
//    SENS_IR_LED.set();

    HAL_TIM_Base_Start_IT(&htim3);

    hal::setup();


    while(1) {
//        VT::print("d");
//        HAL_Delay(10);






//        LED1.toggle();
        LED2.toggle();

//        if (refresh_values) {
//            refresh_values = false;
//            hal::IR_1.set(sensors[0]);
//            IR_2.set(sensors[1]);
//            IR_3.set(sensors[2]);
//            IR_4.set(sensors[3]);
//            IR_5.set(sensors[4]);
//            IR_6.set(sensors[5]);
//            IR_7.set(sensors[6]);
//            IR_8.set(sensors[7]);
//
////            VT::move_to(0, 25);
//////            VT::print((int)(line_detector.calculate_line_position() * 1000.0));
////            VT::print((int)(actual_line_position * 1000.0));
////            VT::print("  ");
//        }

        if (_10Hz_flag) {
            _10Hz_flag = false;
            _10Hz();
        }
    }
}

extern "C"
void My_SysTick_Handler() {
    static uint_fast16_t dd = 0;
    dd++;
    if (dd == 200) {
        refresh_values = true;
        dd = 0;
    }
}

void _10Hz() {
//    int R_data = -zumo().encoderR.encoderGetCountAndReset();
//    int L_data = zumo().encoderL.encoderGetCountAndReset();
//    Encoder_R.set(R_data);
//    Encoder_L.set(L_data);

//    float ret_R = pid_R.tick(static_cast<float>(R_data) / 400.0);
//    float ret_L = pid_L.tick(static_cast<float>(L_data) / 400.0);

//    Duty_R.set((int)(ret_R * 100.0));
//    Duty_L.set((int)(ret_L * 100.0));
}

//Mean<float, 5>position;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim3) { // 10kHz
        zumo().encoderR.encoder10kHzTickISR();
        zumo().encoderL.encoder10kHzTickISR();

        static uint16_t divider_10Hz = 0;
        static uint16_t divider_100Hz = 0;

        if (++divider_100Hz == 100 ) {
            divider_100Hz = 0;
            zumo().line_detector.calculate_line_position();
            actual_line_position = zumo().line_detector.get_line_position();

//            if (driver_enable) {
//                zumo().motor_driver.set_differential(0.3, zumo().line_detector.get_line_position()*0.25 );
//            } else {
//                zumo().motor_driver.Motor_A.set_duty_cycle(0);
//                zumo().motor_driver.Motor_B.set_duty_cycle(0);
//            }
        }

        if (++divider_10Hz == 1000) { // 10Hz
            divider_10Hz = 0;
            _10Hz_flag = true;
        }
    }
}