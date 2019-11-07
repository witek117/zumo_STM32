#ifndef ZUMO_COMMANDS_H
#define ZUMO_COMMANDS_H

#include "commands.h"
#include "terminal.h"
#include "ZUMO.h"

namespace commands {
    struct CommandMotor;
    struct CommandTest;

    using command_type = terminal::Terminal<CommandMotor, CommandTest>;

    command_type terminal();

    struct CommandMotor : terminal::Command<float, float> {
        CommandMotor() : Command("mot") { }

        void callback(float m1, float m2) override {
            m1 = 1;
            m2 = 1;
            if (m1 == m2) {
                m2 = 3;
                m1 = m2;
            }
        }
    };

    struct CommandTest : terminal::Command<int> {
        CommandTest() : Command("test") { }
        void callback(int c) override {
            terminal().send((char)c);
        }
    };

}


#endif //ZUMO_COMMANDS_H