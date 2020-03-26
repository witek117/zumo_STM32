#pragma once

#include "cores/commands_interface.h"
template <typename T>
class MCP9700 : CommandsInterface {
    float temperature = 0.0f;
    T& data_reference;
    T ADC_max_value;
    float ADC_supply_value;

    bool enable = false;

    T get_value() {
        return data_reference;
    }

public:
    MCP9700(T& data_reference, T ADC_max_value, float ADC_supply_value) :
            data_reference(data_reference), ADC_max_value(ADC_max_value), ADC_supply_value(ADC_supply_value) { }

    float get_temperature() {
        if (!enable) {
            return 0.0;
        }

        float temp = static_cast<float>(get_value());

        temp *= ADC_supply_value;
        temp /= ADC_max_value;
        temp -= 0.5f;

        temperature = temp * 100.0f;
        return temperature;
    }

    float get_last_temperature() {
        return temperature;
    }

    uint16_t get_temperature_multiplied() {
        get_temperature();
        return uint16_t(temperature * 100.0f);
    }

    void set_enable(bool enable_) override {
        enable = enable_;
    }

    void init() { }
};
