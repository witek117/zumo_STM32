#ifndef ZUMO_MCP9700_H
#define ZUMO_MCP9700_H

#include "commands_interface.h"

template <typename T>
class MCP9700 : CommandsInterface {
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

        float temperature = static_cast<float>(get_value());

        temperature *= ADC_supply_value;
        temperature /= ADC_max_value;
        temperature -= 0.5f;

        return temperature * 100.0f;
    }

    uint16_t get_temperature_multiplied() {
        float temperature = get_temperature();
        return uint16_t(temperature * 100.0f);
    }

    void set_enable(bool enable_) override {
        enable = enable_;
    }

    void init() { }
};


#endif //ZUMO_MCP9700_H
