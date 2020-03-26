#include "hal.h"
#include "command_terminal/command_manager.h"
#include "commands_runner.h"
#include "main.hpp"

Comman_manager_template command_manager(hal::enable_interrupts, hal::disable_interrupts, {
            Command("s?", get_line_value_callback),
            Command("h?", get_hcsro4_value_callback),
            Command("t?", get_mcp9700_value_callback),
            Command("b?", get_bme280_value_callback),
            Command("ma?", get_mpu_accelerometer_value_callback),
            Command("mg?", get_mpu_gyroscope_value_callback),
            Command("m", set_motors_callback),
            Command("s", set_line_enable_callback),
            Command("h", set_hcsr04_enable_callback),
            Command("t", set_mcp9700_enable_callback),
            Command("b", set_bme280_enable_callback),
            Command("ma", set_mpu_accelerometer_enable_callback),
            Command("mg", set_mpu_gyroscope_enable_callback),
            Command("ws", set_ws2812B_value_callback),
            Command("exit", exit_callback),
            Command("test", test_callback)
        });

// executed in a loop
void hal::loop() {
    command_manager.run();
    callbacks_runner(command_manager);
}