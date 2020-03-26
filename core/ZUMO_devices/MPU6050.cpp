#include <cmath>

#include "MPU6050.hpp"

bool MPU6050::init(GyroscopeData::Scale scale, AccelerometerData::Range range) {
    if (read_register_8bit(RegisterNames::WHO_AM_I) != 0x68) {
        return false;
    }

    set_clock_source(ClockSource::PLL_XGYRO);

    accelerometer.set_range(range);
    gyroscope.set_scale(scale);

    set_deep_sleep(false);

    return true;
}

void MPU6050::set_deep_sleep(bool state) {
    write_register_1bit(RegisterNames::PWR_MGMT_1, 6, state);
}

uint8_t MPU6050::read_register_8bit(RegisterNames register_name) {
    return i2c.read(address, uint8_t(register_name));
}

void MPU6050::write_register_8bit(RegisterNames register_name, uint8_t value) {
    i2c.write(address, uint8_t(register_name), value);
}

int16_t MPU6050::read_register_16bit(RegisterNames register_name) {
    uint8_t * data = i2c.read(address, uint8_t(register_name), 2);
    return (uint16_t(data[0] << 8u) | data[1]);
}

void MPU6050::write_register_16bit(RegisterNames register_name, int16_t value) {
    i2c.write(address, uint8_t(register_name), uint16_t(value));
}

bool MPU6050::read_register_1bit(RegisterNames register_name, uint8_t position) {
    uint8_t value;
    value = read_register_8bit(register_name);
    return (uint8_t(value >> position) & 1u);
}

void MPU6050::write_register_1bit(RegisterNames register_name, uint8_t position, bool state) {
    uint8_t value;
    value = read_register_8bit(register_name);

    if (state) {
        value |= (1u << position);
    } else {
        value &= ~(1u << position);
    }

    write_register_8bit(register_name, value);
}