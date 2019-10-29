#ifndef ZUMO_KTIR0711S_H
#define ZUMO_KTIR0711S_H

#include "GPIO/GPIO.h"
#include <array>
#include <gsl/span>
#include <cmath>

template <typename T, int sensors_number>
class LineSensorsBase {
public:
    virtual std::array<float, sensors_number> get_all_normalized_data() = 0;
};

template <typename T, int max_detected_value, int sensors_number>
class LineSensors : public LineSensorsBase<T, sensors_number> {

    gsl::span<T> head_data_pointer;
    std::array<float, sensors_number> sensors_normalized_data;

public:
    LineSensors(gsl::span<T> head_data_pointer) :
            head_data_pointer(head_data_pointer) {}

    T get_single_raw_value(size_t index) {
        if (index < sensors_number) {
            return head_data_pointer[index];
        }
        return static_cast<T>(0);
    }

    float get_single_normalized_value(size_t index, T max = max_detected_value) {
        float ret_value = 0.0f;
        if (index < sensors_number) {
            T single_data = max - head_data_pointer[index];
            ret_value = static_cast<float>(single_data) / static_cast<float>(max);
        }
        if (ret_value > 1.0f) {
            return 1.0f;
        }
        return ret_value;
    }

    std::array<float, sensors_number> get_all_normalized_data() {
        for (int i = 0; i < sensors_number; i++) {
            sensors_normalized_data[i] = get_single_normalized_value(i);
        }
        return sensors_normalized_data;
    }
};

template <typename T, int sensors_number>
class LineDetector {
    constexpr void calculate_sensors_weights() {
        if constexpr ((sensors_number % 2) == 0) {
            int multiplier = 1;
            for (size_t i = (sensors_number/2); i > 0; i--) {
                sensors_weights.at(sensors_number - i) = multiplier;
                sensors_weights.at(i-1) = -multiplier;
                multiplier++;
            }
        } else {
            int multiplier = 0;
            sensors_weights.at((sensors_number / 2)) = multiplier++;
            for (size_t i = (sensors_number/2); i > 0; i--) {
                sensors_weights.at(sensors_number - i) = multiplier;
                sensors_weights.at(i-1) = -multiplier;
                multiplier++;
            }
        }
    }
public:
    enum class LineStatus {
        DETECTED,
        LOST_ON_RIGHT,
        LOST_ON_LEFT,
        UNKNOWN
    };

protected:
    LineSensorsBase<T, sensors_number> &sensors_line;
    std::array<int, sensors_number> sensors_weights;
    float line_position;
    std::array<float, 5> line_history;
    bool line_detected = false;
    LineStatus line_status;

public:
    LineDetector(LineSensorsBase<T, sensors_number> &sensors_line) : sensors_line(sensors_line),
                                                                     line_position(0),
                                                                     line_history({0, 0, 0, 0, 0}),
                                                                     line_status(LineStatus::UNKNOWN) {
            calculate_sensors_weights();
    }

    std::array<int, sensors_number> get_sensors_weights() {
        return sensors_weights;
    }

    float calculate_line_position() {
        line_position = 0.0f;
        float sensors_sum = 0.0f;

        auto data = sensors_line.get_all_normalized_data();

        for(auto item : data) {
            sensors_sum += item;
        }

        if (sensors_sum >= 1.0) { // line detected
//            line_detected = true;
            line_status = LineStatus::DETECTED;
            for(size_t i = 0; i < data.size(); i++) {
                line_position += data[i] * sensors_weights[i] / sensors_sum;
            }
            update_line_history();

        } else { // line not detected
            // check na what side was last line detection
            line_position = std::round(line_history.at(0)) * 10.0;
            if ( line_position > 0) {
                line_status = LineStatus::LOST_ON_RIGHT;
            } else {
                line_status = LineStatus::LOST_ON_LEFT;
            }
        }
        return line_position;
    }

    void update_line_history() {
        float round_line_position = std::round(line_position);

        if (round_line_position != line_history.at(0)) {
            for (int i = line_history.size() - 1; i > 0; i--) {
                line_history.at(i) = line_history.at(i - 1);
            }
            line_history.at(0) = round_line_position;
        }
    }

//    void follow_line() {
//        if (!line_detected) return;
//
//        float first_value = line_history.at(0);
//        std::array<float, 3> possible_values = {
//                std::round(first_value - 1),
//                std::round(first_value),
//                std::round(first_value + 1)
//        };
//
////        bool line_detected = false;
//
//        for (auto value : possible_values ) {
//            if (value == line_history.at(1)) {
////                line_detected = true;
//                break;
//            }
//        }
//
////        std::cout << line_detected << std::endl;
//    }



};


#endif //ZUMO_KTIR0711S_H
