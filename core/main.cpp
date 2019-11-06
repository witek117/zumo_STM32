#include "hal.h"
#include "command_terminal/fifo.h"
#include "command_terminal/commands.h"
#include "window_terminal/window_manager.hpp"

std::vector<Window> windows;

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

FIFO<char, 1000> fifo;
volatile uint8_t commands_in_fifo = 0;

char *copy_from_fifo_to_buffer();

void hal::receive_char_interrupt(char chr) {
    fifo.append(chr);
    if (chr == '\n') {
        commands_in_fifo++;
    }
}

Label IR_1("1", "",false, nullptr, 1, 0, 4095);
Label IR_2("2", "",false, nullptr, 1, 0, 4095);
Label IR_3("3", "",false, nullptr, 1, 0, 4095);
Label IR_4("4", "",false, nullptr, 1, 0, 4095);
Label IR_5("5", "",false, nullptr, 1, 0, 4095);
Label IR_6("6", "",false, nullptr, 1, 0, 4095);
Label IR_7("7", "",false, nullptr, 1, 0, 4095);
Label IR_8("8", "",false, nullptr, 1, 0, 4095);

// executed once, after hardware initialization
void hal::setup() {
    windows.emplace_back(Window("Motor",  1, 1, 10, 5, true));
    windows.emplace_back(Window("IR",  13, 1, 16, 17, false));
    windows.emplace_back(Window("ENC",  31, 1, 16, 5, false));
    windows.emplace_back(Window("Duty",  31, 7, 16, 5, false));
    windows.emplace_back(Window("Nothing",  0, 19, 16, 5, true));

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

    MotorBrakeA.add_text("ON");
    MotorBrakeA.add_text("OFF");

    MotorBrakeB.add_text("ON");
    MotorBrakeB.add_text("OFF");

    windows[0].add_box(&MotorBrakeA);
    windows[0].add_box(&MotorBrakeB);
    MotorBrakeA.increase();
    MotorBrakeB.increase();

    zumo().motor_driver.Motor_A.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
    zumo().motor_driver.Motor_B.set_mode(DRV8833::MotorChannel::Mode::REVERSE_FAST_DECAY);
    zumo().motor_driver.Motor_A.set_duty_cycle(0.7);
    zumo().motor_driver.Motor_B.set_duty_cycle(0.7);
    zumo().motor_driver.Motor_A.brake();
    zumo().motor_driver.Motor_B.brake();

    window_manager::init(&windows);
    window_manager::refresh_all();
}

// executed in a loop
void hal::loop() {
    hal::disable_interrupts();
    uint8_t commands_in_fifo_local = commands_in_fifo;
    commands_in_fifo = 0;
    hal::enable_interrupts();

    while (commands_in_fifo_local--) {
        char* cmd_buffer = copy_from_fifo_to_buffer();
        commands::terminal().parse_line(cmd_buffer);
    }

    window_manager::run();
}


char *copy_from_fifo_to_buffer() {
    static std::array<char, 100> cmd_buffer;

    auto it = cmd_buffer.begin();

    hal::disable_interrupts();
    while (fifo.size() != 0 && it != cmd_buffer.end()) {
        *it = fifo.get();
        if(*it == '\n') {
            *it = '\0';
            break;
        }
        it++;
    }
    hal::enable_interrupts();
    return cmd_buffer.data();
}