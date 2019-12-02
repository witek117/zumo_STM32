#include "commands_runner.h"
#include "ZUMO.h"
#include "window_terminal/label.hpp"

extern Label IR_1;
extern Label IR_2;
extern Label IR_3;
extern Label IR_4;
extern Label IR_5;
extern Label IR_6;
extern Label IR_7;
extern Label IR_8;

std::array<Label*, 8> labels = {&IR_1, &IR_2, &IR_3, &IR_4, &IR_5, &IR_6, &IR_7, &IR_8};

volatile bool get_sensors_flag = false;
void get_sensors_callback(const char* data){
    (void)data;
    get_sensors_flag = true;
    zumo().LED1.toggle();
}

volatile bool test_flag = false;
void test_callback(const char* data){
    (void)data;
    test_flag = true;
}

void set_motors_callback(const char* data) {
    auto [m1, m2] = parser::get<float, float>(data);
    zumo().motor_driver.Motor_A.set_duty_cycle(m1);
    zumo().motor_driver.Motor_B.set_duty_cycle(m2);
}

void set_line_enable_callback(const char* data) {
    auto [l] = parser::get<int>(data);
    if (l == 1) {
        zumo().line_sensors.init();
    } else {
        zumo().line_sensors.deinit();
    }
}

volatile bool exit_flag = false;
void exit_callback(const char* data) {
    (void)data;
    exit_flag = true;
}

void set_hcsr04_enable_callback(const char* data) {
    auto [l] = parser::get<int>(data);
    if (l == 1) {
        zumo().hcsr04.init();
    } else {
        zumo().hcsr04.deinit();
    }
}

volatile bool hcsr04_flag = false;

void get_hcsro4_value_callback(const char*) {
    hcsr04_flag = true;
}

void callbacks_runner(PrintManager& command_manager) {
    if (get_sensors_flag) {
        command_manager.print('S');
        volatile unsigned short * data = zumo().line_sensors.get_data_pointer();
        for (int i =0; i < zumo().line_sensors.size(); i ++) {
            uint16_t val = *data++;
            command_manager.print(static_cast<char>(val >> 8u));
            command_manager.print(static_cast<char>(0x00FFu & val));
            labels[i]->set(val);
        }
        get_sensors_flag = false;
    }

    if (test_flag) {
        test_flag = false;
        command_manager.print("1234");
    }

    if (exit_flag) {
        exit_flag = false;
        command_manager.deinit();
    }

    if (hcsr04_flag) {
        hcsr04_flag = false;
        command_manager.print('h');
        auto val = zumo().hcsr04.get_value();
        command_manager.print(static_cast<char>(val >> 8u));
        command_manager.print(static_cast<char>(0x00FFu & val));
    }

}