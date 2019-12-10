#include "hal.h"
#include "window_terminal/window_manager.hpp"
#include "command_terminal/command_manager.h"
#include "ZUMO.h"
#include "commands_runner.h"


std::vector<Window> windows;

CommandManager <11, '\r', false> command_manager(hal::enable_interrupts, hal::disable_interrupts, {
            Command("s?", get_line_value_callback),         // get sensors values
            Command("h?", get_hcsro4_value_callback),       // get HC-SR04 last value
            Command("t?", get_mcp9700_value_callback),      // get last temperature value
            Command("b?", get_bme280_value_callback),       // get last temperature value
            Command("m", set_motors_callback),              // set motors duty cycle
            Command("s", set_line_enable_callback),
            Command("h", set_hcsr04_enable_callback),
            Command("t", set_mcp9700_enable_callback),
            Command("b", set_bme280_enable_callback),
            Command("exit", exit_callback),                 // exit from command_manager
            Command("test", test_callback)                  // test callback
        });


void change_motor_brakeA();
Text MotorBrakeA("L", true, change_motor_brakeA);
void change_motor_brakeA () {
    if (MotorBrakeA.get_value_index() == 0) {
        zumo().motor_driver.Motor_A.run();
    } else {
        zumo().motor_driver.Motor_A.brake();
    }
}

void change_motor_brakeB();
Text MotorBrakeB("R", true, change_motor_brakeB);
void change_motor_brakeB () {
    if (MotorBrakeB.get_value_index() == 0) {
        zumo().motor_driver.Motor_B.run();
    } else {
        zumo().motor_driver.Motor_B.brake();
    }
}

Label Duty_R("R", "", false, nullptr, 100, 0, 100);
Label Duty_L("L", "", false, nullptr, 100, 0, 100);

Label Encoder_L("L", "", false, nullptr, 1, -1000, 1000);
Label Encoder_R("R", "", false, nullptr, 1, -1000, 1000);

volatile bool driver_enable = false;
void change_motors_enable() ;
Text MotorsEnable("EN",true, change_motors_enable);
void change_motors_enable() {
    driver_enable = MotorsEnable.get_value_index() != 0;
}

Label IR_1("1", "",false, nullptr, 1, 0, 4095);
Label IR_2("2", "",false, nullptr, 1, 0, 4095);
Label IR_3("3", "",false, nullptr, 1, 0, 4095);
Label IR_4("4", "",false, nullptr, 1, 0, 4095);
Label IR_5("5", "",false, nullptr, 1, 0, 4095);
Label IR_6("6", "",false, nullptr, 1, 0, 4095);
Label IR_7("7", "",false, nullptr, 1, 0, 4095);
Label IR_8("8", "",false, nullptr, 1, 0, 4095);

Label HC("HC", "",false, nullptr, 1, 0, 4095);
// executed once, after hardware initialization
void hal::setup() {
    windows.emplace_back(Window("Motor",  1, 1, 10, 5, true));

    MotorBrakeA.add_text("ON");
    MotorBrakeA.add_text("OFF");
//
    MotorBrakeB.add_text("ON");
    MotorBrakeB.add_text("OFF");
//
    windows[0].add_box(&MotorBrakeA);
    windows[0].add_box(&MotorBrakeB);
    MotorBrakeA.increase();
    MotorBrakeB.increase();

    windows.emplace_back(Window("IR",  13, 1, 16, 17, false));
    windows.emplace_back(Window("ENC",  31, 1, 16, 5, false));
    windows.emplace_back(Window("Duty",  31, 7, 16, 5, false));
    windows.emplace_back(Window("Nothing",  0, 19, 16, 8, true));

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

    MotorsEnable.add_text("OFF");
    MotorsEnable.add_text("ON");

    windows[4].add_box(&MotorsEnable);
    windows[4].add_box(&HC);

    zumo().motor_driver.Motor_A.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
    zumo().motor_driver.Motor_B.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
    zumo().motor_driver.Motor_A.set_duty_cycle(0.7);
    zumo().motor_driver.Motor_B.set_duty_cycle(0.7);
    zumo().motor_driver.Motor_A.brake();
    zumo().motor_driver.Motor_B.brake();

    window_manager::init(&windows);
}

// executed in a loop
void hal::loop() {
    command_manager.run();
    window_manager::run();

    callbacks_runner(command_manager);
    HC.set(zumo().hcsr04.get_last_value());
}