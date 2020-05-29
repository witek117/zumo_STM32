#pragma once

#include <cstdint>

class HALina_I2C {
public:
    virtual void write(uint8_t address, uint8_t *data, uint8_t len_data) = 0;
    virtual void write(uint8_t address, uint8_t register_address, uint8_t data) = 0;
    virtual void write(uint8_t address, uint8_t register_address, uint16_t data) = 0;
    virtual void write(uint8_t address, uint8_t register_address) = 0;
    virtual uint8_t read(uint8_t address, uint8_t register_address) = 0;
    virtual uint8_t* read(uint8_t address, uint8_t register_address, size_t size) = 0;
};

