#ifndef ZUMO_COMMANDS_RUNNER_H
#define ZUMO_COMMANDS_RUNNER_H

#include "command_terminal/command_manager.h"

void callbacks_runner(PrintManager& command_manager);

// command test
void test_callback(const char*);

// motors
void set_motors_callback(const char*);

// line
void set_line_enable_callback(const char*);
void get_line_value_callback(const char*);

// HC-SR04
void set_hcsr04_enable_callback(const char*);
void get_hcsro4_value_callback(const char*);

// MCP9700
void set_mcp9700_enable_callback(const char*);
void get_mcp9700_value_callback(const char*);

// BME280
void set_bme280_enable_callback(const char*);
void get_bme280_value_callback(const char*);

// MPU6050
void get_mpu_accelerometer_value_callback(const char*);
void get_mpu_gyroscope_value_callback(const char*);
void set_mpu_accelerometer_enable_callback(const char*);
void set_mpu_gyroscope_enable_callback(const char*);

// WS2815B
void set_ws2812B_value_callback(const char*);

// exit, turn off all
void exit_callback(const char*);
#endif //ZUMO_COMMANDS_RUNNER_H
