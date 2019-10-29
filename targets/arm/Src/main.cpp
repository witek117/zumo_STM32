#include "main.h"
#include "hal.h"
#include "window_terminal/window_manager.hpp"
#include "vector"
#include "DRV8833.h"
#include "encoder.h"
#include "PID.h"
#include "KTIR0711S.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

//extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
uint8_t Received;
void usart_put_char(char ch) { HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 10); }

STM32_GPIO LED1(LED1_GPIO_Port, LED1_Pin);

extern "C"
void handle_usart_interrupt(UART_HandleTypeDef *huart) {
    uint32_t isrflags   = READ_REG(huart->Instance->SR);
    uint32_t cr1its     = READ_REG(huart->Instance->CR1);
//    uint32_t cr3its     = READ_REG(huart->Instance->CR3);
    uint32_t errorflags = 0x00U;
//    uint32_t dmarequest = 0x00U;

    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    if (errorflags == RESET) {
        /* UART in mode Receiver -------------------------------------------------*/
        if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)) {
            window_manager::in_RX_callback(static_cast<uint8_t >(huart->Instance->DR));


            return;
        }
    }
}

std::vector<Window> windows;

STM32_PWM<1000> PWM_1_CH1(TIM1->CCR1);
STM32_PWM<1000> PWM_1_CH2(TIM1->CCR2);


STM32_PWM<1000> PWM_2_CH3(TIM2->CCR3);
STM32_PWM<1000> PWM_2_CH4(TIM2->CCR4);
//
//// DRV8833
STM32_GPIO fault(FAULT_GPIO_Port, FAULT_Pin);
STM32_GPIO_FAKE nSleep;
DRV8833 motor_driver(PWM_1_CH1, PWM_1_CH2, PWM_2_CH3, PWM_2_CH4, nSleep, fault);
//
//
void change_motor_brakeA();
Text MotorBrakeA("L", true, change_motor_brakeA);
void change_motor_brakeA () {
    if (MotorBrakeA.get_value_index() == 0) {
        motor_driver.Motor_A.run();
    } else {
        motor_driver.Motor_A.brake();
    }
}

void change_motor_brakeB();
Text MotorBrakeB("R", true, change_motor_brakeB);
void change_motor_brakeB () {
    if (MotorBrakeB.get_value_index() == 0) {
        motor_driver.Motor_B.run();
    } else {

        motor_driver.Motor_B.brake();
    }
}


volatile uint16_t sensors[8];
volatile bool refresh_values = false;

STM32_GPIO MOT_L_A(MOT_L_A_GPIO_Port, MOT_L_A_Pin);
STM32_GPIO MOT_L_B(MOT_L_A_GPIO_Port, MOT_L_B_Pin);

STM32_GPIO MOT_R_A(MOT_R_A_GPIO_Port, MOT_R_A_Pin);
STM32_GPIO MOT_R_B(MOT_R_A_GPIO_Port, MOT_R_B_Pin);

Encoder MOT_L(MOT_L_A, MOT_L_B, 1);
Encoder MOT_R(MOT_R_A, MOT_R_B, 1);

Label Encoder_L("L", "", false, nullptr, 1, -1000, 1000);
Label Encoder_R("R", "", false, nullptr, 1, -1000, 1000);

volatile bool _10Hz_flag = false;
void _10Hz();

PID pid_L(0.4, 0.1, 0.01, 1, 1, -1, 1);
PID pid_R(0.4, 0.1, 0.01, 1, 1, -1, 1);

Label Duty_R("R", "", false, nullptr, 100, 0, 100);
Label Duty_L("L", "", false, nullptr, 100, 0, 100);

extern "C"
void Main() {
    // LED configuration

    STM32_GPIO LED2(LED2_GPIO_Port, LED2_Pin);

    STM32_GPIO SENS_IR_LED(SENS_IR_LED_GPIO_Port, SENS_IR_LED_Pin);

    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    // terminal configuration
    VT::init(usart_put_char);

    windows.emplace_back(Window("Motor",  1, 1, 10, 5, true));
    windows.emplace_back(Window("IR",  13, 1, 16, 17, false));
    windows.emplace_back(Window("ENC",  31, 1, 16, 5, false));
    windows.emplace_back(Window("Duty",  31, 7, 16, 5, false));

    Label IR_1("1", "",false, nullptr, 1, 0, 4095);
    Label IR_2("2", "",false, nullptr, 1, 0, 4095);
    Label IR_3("3", "",false, nullptr, 1, 0, 4095);
    Label IR_4("4", "",false, nullptr, 1, 0, 4095);
    Label IR_5("5", "",false, nullptr, 1, 0, 4095);
    Label IR_6("6", "",false, nullptr, 1, 0, 4095);
    Label IR_7("7", "",false, nullptr, 1, 0, 4095);
    Label IR_8("8", "",false, nullptr, 1, 0, 4095);

    windows[1].add_box(&IR_1);
    windows[1].add_box(&IR_2);
    windows[1].add_box(&IR_3);
    windows[1].add_box(&IR_4);
    windows[1].add_box(&IR_5);
    windows[1].add_box(&IR_6);
    windows[1].add_box(&IR_7);
    windows[1].add_box(&IR_8);

    windows[2].add_box(&Encoder_L);
    windows[2].add_box(&Encoder_R);

    windows[3].add_box(&Duty_L);
    windows[3].add_box(&Duty_R);

//    HAL_UART_Receive_DMA(&huart1, &Received, 1);

    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

    MotorBrakeA.add_text("ON");
    MotorBrakeA.add_text("OFF");

    MotorBrakeB.add_text("ON");
    MotorBrakeB.add_text("OFF");

    windows[0].add_box(&MotorBrakeA);
    windows[0].add_box(&MotorBrakeB);
    MotorBrakeA.increase();
    MotorBrakeB.increase();

    window_manager::init(&windows);
    window_manager::refresh_all();

    motor_driver.Motor_A.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
    motor_driver.Motor_B.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
    motor_driver.Motor_A.set_duty_cycle(0.9);
    motor_driver.Motor_B.set_duty_cycle(0.9);
    motor_driver.Motor_A.brake();
    motor_driver.Motor_B.brake();

    LED1.set();
    LED2.reset();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensors, 8);
    SENS_IR_LED.set();

    HAL_TIM_Base_Start_IT(&htim3);

    LineSensors <volatile uint16_t , 4095, 8>line_sensors(sensors);
    LineDetector<volatile uint16_t, 8> line_detector(line_sensors);

    while(1) {
        window_manager::run();
//        LED1.toggle();
        LED2.toggle();

        if (refresh_values) {
            refresh_values = false;
            IR_1.set(sensors[0]);
            IR_2.set(sensors[1]);
            IR_3.set(sensors[2]);
            IR_4.set(sensors[3]);
            IR_5.set(sensors[4]);
            IR_6.set(sensors[5]);
            IR_7.set(sensors[6]);
            IR_8.set(sensors[7]);

            VT::move_to(0, 25);
            VT::print((int)(line_detector.calculate_line_position() * 1000.0));
            VT::print("  ");
        }

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
    if (dd == 500) {
        refresh_values = true;
        dd = 0;
    }
}

void _10Hz() {
    int R_data = -MOT_R.encoderGetCountAndReset();
    int L_data = MOT_L.encoderGetCountAndReset();
    Encoder_R.set(R_data);
    Encoder_L.set(L_data);

    float ret_R = pid_R.tick(static_cast<float>(R_data) / 400.0);
    float ret_L = pid_L.tick(static_cast<float>(L_data) / 400.0);

    Duty_R.set((int)(ret_R * 100.0));
    Duty_L.set((int)(ret_L * 100.0));
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim3) { // 10kHz
        MOT_R.encoder10kHzTickISR();
        MOT_L.encoder10kHzTickISR();

        static uint16_t divider = 0;
        if (++divider == 1000) { // 10Hz
            divider = 0;
            _10Hz_flag = true;
        }
    }
}