#include "commands.h"


namespace commands {
    command_type terminal() {
        static CommandMotor command_motor;

        static CommandTest command_test;

        static auto terminal = terminal::Terminal(command_motor, command_test);

        return terminal;
    }
}