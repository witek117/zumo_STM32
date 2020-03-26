#pragma once

namespace hal {
    void enable_interrupts();
    void disable_interrupts();

// ------------------- implemented by core -------------------
    void receive_char_interrupt(char chr);

// executed once, after hardware initialization
    void setup();

// executed in a loop
    void loop();
}

