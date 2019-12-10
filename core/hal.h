#pragma once

#include <gsl/gsl>

namespace hal {

    class I2C {
    public:
        virtual void write(uint8_t address, uint8_t register_address, uint8_t data) = 0;
        virtual void write(uint8_t address, uint8_t register_address) = 0;
        virtual uint8_t read(uint8_t address, uint8_t register_address) = 0;
        virtual uint8_t* read(uint8_t address, uint8_t register_address, ssize_t size) = 0;
    };

    class GPIO {
    public:
        enum class Mode : uint8_t {
            INPUT,
            OUTPUT,
            TIMER,
        };

        virtual void set() = 0;
        virtual void reset() = 0;
        virtual bool get() = 0;
        virtual void toggle() = 0;
    };

    class PWM {
    public:
        virtual void set_duty_cycle(float) = 0;
        virtual void off() = 0;
        virtual void on() = 0;
        virtual float get_duty_cycle() = 0;
    };


    void enable_interrupts();
    void disable_interrupts();

// ------------------- implemented by core -------------------

    void receive_char_interrupt(char chr);

// executed once, after hardware initialization
    void setup();

// executed in a loop
    void loop();
}

