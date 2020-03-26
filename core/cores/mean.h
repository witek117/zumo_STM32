#ifndef ZUMO_MEAN_H
#define ZUMO_MEAN_H

#include <array>

template <typename T, int size>
class Mean {
    bool is_full;
    size_t index;
    std::array<T, size> data {};
    T value;
public:
    Mean() : is_full(false), index(0), value(0) { }

    std::array<T, size> get_all_data() {
        return data;
    }

    void put_value(T new_value, uint8_t divider = 1) {
        static uint8_t actual_divider = 0;

        if (++actual_divider == divider) {
            actual_divider = 0;

            index = (index + 1) % size;
            if (index == 0) {
                is_full = true;
            }
            data.at(index) = new_value;
        }

        T mean = static_cast<T>(0);
        for (auto val : data) {
            mean += val;
        }

        if (is_full) {
            value =  mean / static_cast<T>(size);
        } else {
            value = mean / static_cast<T>(index);
        }
    }

    T get_value() {
        return value;
    }
};

#endif //ZUMO_MEAN_H
