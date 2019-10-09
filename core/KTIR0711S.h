#ifndef ZUMO_KTIR0711S_H
#define ZUMO_KTIR0711S_H

#include "GPIO/GPIO.h"
#include <array>
#include <iostream>
#include <gsl/span>

template <typename T, int max_detected_value>
class KTIR0711S {
public:
    T &data_pointer;
    KTIR0711S(T &data_pointer) : data_pointer(data_pointer) {

    }

    T get_raw_value() {
        return data_pointer;
    }

    float get_normalized_value() {
        if (data_pointer > max_detected_value) {
            return 1.00;
        }
        return (float)data_pointer / (float)max_detected_value;
    }

    float get_normalized_value(float max_detected_value_given) {
        if (data_pointer > max_detected_value_given) {
            return 1.00;
        }
        return (float)data_pointer / max_detected_value_given;
    }
};

namespace LineSensorsNamespace {
    enum class FindValue {
        SMALL,
        BIG
    };
}

template <typename T, int max_detected_value, int sensors_number>
class LineSensors {
    using Sensor = KTIR0711S<T, max_detected_value>;

    gsl::span<T> head_data_pointer;
    std::array<Sensor, sensors_number> Sensors;

    template<int now, int max>
    auto get() {
        if constexpr (now < max) {
            return std::tuple_cat(std::tuple<Sensor>(Sensor(head_data_pointer[now])), get<now+1, max>());
        } else {
            return std::tuple<>();
        }
    }

    template<typename tuple_t>
    constexpr auto get_array_from_tuple(tuple_t&& tuple) {
        constexpr auto get_array = [](auto&& ... x){ return std::array{std::forward<decltype(x)>(x) ... }; };
        return std::apply(get_array, std::forward<tuple_t>(tuple));
    }

public:
    LineSensorsNamespace::FindValue find_value;
// 0..sensors_number -1
// S(head_data_pointer[i])

    LineSensors(gsl::span<T> head_data_pointer, LineSensorsNamespace::FindValue find_value):
        head_data_pointer(head_data_pointer),
        Sensors(get_array_from_tuple(get<0, sensors_number>())),
        find_value(find_value) {}

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
};

#endif //ZUMO_KTIR0711S_H
