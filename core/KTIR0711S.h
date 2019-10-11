#ifndef ZUMO_KTIR0711S_H
#define ZUMO_KTIR0711S_H

#include "GPIO/GPIO.h"
#include <array>
#include <iostream>
#include <gsl/span>

template <typename T, T max_detected_value>
class KTIR0711S {
    T &data_pointer;
public:
    constexpr  KTIR0711S(T &data_pointer) : data_pointer(data_pointer) {

    }

    auto get_raw_value() const {
        return data_pointer;
    }

    float get_normalized_value(T max = max_detected_value) const {
        return (data_pointer <= max) ?  ((1.0 * data_pointer) / max) : 1.0;
    }
};


template <typename T, int sensors_number>
class LineSensorsBase {
public:
    virtual std::array<float, sensors_number> get_sensors_data() = 0;
};

template <typename T, int max_detected_value, int sensors_number>
class LineSensors : public LineSensorsBase<T, sensors_number> {
    using Sensor = KTIR0711S<T, max_detected_value>;
    T table_data[sensors_number];

    gsl::span<T> head_data_pointer;
    std::array<Sensor, sensors_number> Sensors;

    std::array<float, sensors_number> sensors_normalized_data;

    template<int now, int max>
    auto get() const{
        if constexpr (now < max) {
            return std::tuple_cat(std::tuple<Sensor>(Sensor(head_data_pointer[now])), get<now + 1, max>());
        } else {
            return std::tuple<>();
        }
    }

    template<typename tuple_t>
    constexpr auto get_array_from_tuple(tuple_t &&tuple) {
        constexpr auto get_array = [](auto &&... x) { return std::array{std::forward<decltype(x)>(x) ...}; };
        return std::apply(get_array, std::forward<tuple_t>(tuple));
    }

public:
//    LineSensorsNamespace::FindValue find_value;
// 0..sensors_number -1
// S(head_data_pointer[i])

    constexpr LineSensors(gsl::span<T> head_data_pointer) :
            head_data_pointer(head_data_pointer),
            Sensors(get_array_from_tuple(get<0, sensors_number>())) {}

    T get_single_raw_value(size_t index) {
        if (index < sensors_number) {
            return Sensors[index].get_raw_value();
        }
        return static_cast<T>(0);
    }

    float get_single_normalized_value(size_t index) {
        if (index < sensors_number) {
            return Sensors[index].get_normalized_value();
        }
        return static_cast<T>(0);
    }

    float get_single_normalized_value(size_t index, float max_detected_value_given) {
        if (index < sensors_number) {
            return Sensors[index].get_normalized_value(max_detected_value_given);
        }
        return static_cast<T>(0);
    }

    std::array<float, sensors_number> get_all_values() {
        std::array<float, sensors_number> ret_array;
        for (int i = 0; i < sensors_number; i++) {
            ret_array.at(i) = get_single_normalized_value(i);
        }
        return ret_array;
    }

    std::array<T, sensors_number> get_all_raw_values() {
        std::array<T, sensors_number> ret_array;
        for (int i = 0; i < sensors_number; i++) {
            ret_array.at(i) = get_single_raw_value(i);
        }
        return ret_array;
    }

    std::array<float, sensors_number> get_sensors_data() {
        for (int i = 0; i < sensors_number; i++) {
            sensors_normalized_data[i] = Sensors[i].get_normalized_value();
        }
        return sensors_normalized_data;
    }
};



template <typename T, int sensors_number>
class LineDetector {
    LineSensorsBase<T, sensors_number> &sensors_line;
    std::array<int, sensors_number> sensors_weights;
public:
    LineDetector(LineSensorsBase<T, sensors_number> &sensors_line) : sensors_line(sensors_line), sensors_weights(get_sensors_weights()){

    }

    constexpr std::array<int, sensors_number> get_sensors_weights() {
        std::array<int, sensors_number> weights;
        if constexpr ((sensors_number % 2) == 0) {
            int multiplier = 1;
            for (size_t i = (sensors_number/2); i > 0; i--) {
                weights.at(sensors_number - i) = multiplier;
                weights.at(i-1) = -multiplier;
                multiplier++;
            }
        } else {
            int multiplier = 0;
            weights.at((sensors_number / 2)) = multiplier++;
            for (size_t i = (sensors_number/2); i > 0; i--) {
                weights.at(sensors_number - i) = multiplier;
                weights.at(i-1) = -multiplier;
                multiplier++;
            }
        }
        return weights;
    }

    float calculate_line_position() {
        float line_position = 0.0f;
        auto data = sensors_line.get_sensors_data();
        float sensors_sum = 0.0f;
        for(auto item : data) {
            sensors_sum += item;
        }

        for(size_t i = 0; i < data.size(); i++) {
            line_position += data[i] * sensors_weights[i] / sensors_sum;
        }
        return line_position;
    }
};


#endif //ZUMO_KTIR0711S_H
