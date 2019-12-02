#ifndef ZUMO_COMMANDS_RUNNER_H
#define ZUMO_COMMANDS_RUNNER_H

#include "command_terminal/command_manager.h"

void callbacks_runner(PrintManager& command_manager);

void test_callback(const char*);
void get_sensors_callback(const char*);
void set_motors_callback(const char*);
void set_line_enable_callback(const char*);
void exit_callback(const char*);
void set_hcsr04_enable_callback(const char*);
void get_hcsro4_value_callback(const char*);

#endif //ZUMO_COMMANDS_RUNNER_H
