#ifndef ZUMO_COMMANDS_H
#define ZUMO_COMMANDS_H

#include "terminal.h"
#include "ZUMO.h"

namespace commands {
    struct CommandMotor : terminal::Command<float, float> {
        CommandMotor() : Command("mot") {}

        void callback(float m1, float m2) override {
            m1 = 1;
            m2 = 1;
            if (m1 == m2) {
                m2 = 3;
                m1 = m2;
            }
//            &zumo().motor_driver.Motor_A.set_duty_cycle(m1);
//            &zumo().motor_driver.Motor_B.set_duty_cycle(m2);

        }
    };

    using command_type = terminal::Terminal<CommandMotor>;

    command_type terminal();
}


#endif //ZUMO_COMMANDS_H