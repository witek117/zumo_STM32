#include "commands.h"


namespace commands {
    command_type terminal() {
        static CommandMotor command_motor;
        
        static auto terminal = terminal::Terminal(command_motor);

        return terminal;
    }
}