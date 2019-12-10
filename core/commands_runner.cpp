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

bool static get_enable(const char* data) {
    auto [l] = parser::get<int>(data);
    return (1 == l);
}

volatile bool get_sensors_flag = false;
void get_line_value_callback(const char* data){
    (void)data;
    get_sensors_flag = true;
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
    zumo().line_sensors.set_enable(get_enable(data));
}

volatile bool exit_flag = false;
void exit_callback(const char* data) {
    (void)data;
    exit_flag = true;
}

void set_hcsr04_enable_callback(const char* data) {
    zumo().hcsr04.set_enable(get_enable(data));
}

volatile bool hcsr04_flag = false;

void get_hcsro4_value_callback(const char* data) {
    (void) data;
    hcsr04_flag = true;
}

void set_mcp9700_enable_callback(const char* data) {
    zumo().mcp9700.set_enable(get_enable(data));
}

volatile bool mcp9700_flag = false;
void get_mcp9700_value_callback(const char* data) {
    (void) data;
    mcp9700_flag = true;
}

void set_bme280_enable_callback (const char* data) {
    zumo().bme280.set_enable(get_enable(data));
}

volatile bool bme280_flag = false;
void get_bme280_value_callback(const char* data) {
    (void) data;
    bme280_flag = true;
}

void callbacks_runner(PrintManager& command_manager) {
    if (get_sensors_flag) {
        command_manager.print('s');
        volatile unsigned short * data = zumo().line_sensors.get_data_pointer();
        for (int i =0; i < zumo().line_sensors.size(); i ++) {
            uint16_t val = *data++;
            command_manager.print((uint16_t)val);
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
        command_manager.print((uint16_t)zumo().hcsr04.get_last_value());
    }

    if (mcp9700_flag) {
        mcp9700_flag = false;
        command_manager.print('t');
        command_manager.print((uint16_t)zumo().mcp9700.get_temperature_multiplied());
    }

    if (bme280_flag) {
        bme280_flag = false;
        command_manager.print('b');
        command_manager.print((uint16_t)zumo().bme280.get_last_temperature_multiplied());
        command_manager.print((uint16_t)zumo().bme280.get_last_humidity());
        command_manager.print((uint32_t)zumo().bme280.get_last_pressure());
    }

}