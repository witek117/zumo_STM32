#include <gtest/gtest.h>
#include <tuple>
#include "KTIR0711S.hpp"
#include <gmock/gmock.h>
#include "HALina_GPIO.hpp"
#include "hal.h"

class GPIOMock : public HALina_GPIO {
public:
    MOCK_METHOD(void, set, ());
    MOCK_METHOD(void, reset, ());
    MOCK_METHOD(void, setMode, ());
    MOCK_METHOD(bool, get, ());
    MOCK_METHOD(void, toggle, ());
};


class LineDetectorMOCK : public LineDetector<uint16_t, 8> {
    GPIOMock enable;
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {0, 0, 0, 0, 0, 0, 0, 0};
    LineSensors<uint16_t , 1000, table_len> line;

public:
    LineDetectorMOCK() :  LineDetector(line), line(data, enable) {

    }

    void update_line_position(float line_position) {
        this->line_position.put_value(line_position);
    }

    int get_sensors_number() {
        return sensors_weights.size();
    }

    auto get_line_histiry() {
        return line_history;
    }
};

TEST(KTIR0711S, sensors_line) {
    GPIOMock enable;
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {4,5,6,7,8,9,2,3};
    LineSensors<uint16_t , 1000, table_len> line(data, enable);

    constexpr static float max_value = 500.0;

    for(size_t i = 0; i < table_len; i++) {
        EXPECT_EQ(data[i], line.get_single_raw_value(i));
        EXPECT_EQ((1000.0f - (float)data[i]) / 1000.0f, line.get_single_normalized_value(i));
        EXPECT_EQ((max_value - (float)data[i]) / max_value, line.get_single_normalized_value(i, max_value));
    }
}

TEST(KTIR0711S, sensors_line_too_big_index) {
    GPIOMock enable;
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {4,5,6,7,8,9,2,3};
    LineSensors<uint16_t , 1000, table_len> line(data, enable);

    constexpr static float max_value = 500.0;

    for(size_t i = table_len; i < (table_len + 3); i++) {
        EXPECT_EQ(0, line.get_single_raw_value(i));
        EXPECT_EQ(0, line.get_single_normalized_value(i));
        EXPECT_EQ(0, line.get_single_normalized_value(i, max_value));
    }
}

TEST(KTIR0711S, get_all_normalized_data) {
    GPIOMock enable;
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {4,5,6,7,8,9,2,3};
    LineSensors<uint16_t , 1000, table_len> line(data, enable);

    std::array<float, table_len> returned_data = line.get_all_normalized_data();

    for(size_t i = 0; i < table_len ; i++) {
        EXPECT_EQ((1000.0f - static_cast<float>(data[i])) / 1000, returned_data.at(i));
    }
}

TEST(LINE_DETECTOR, get_sensors_weight) {
    GPIOMock enable;
    {
        constexpr static size_t table_len_7 = 7;
        uint16_t data[table_len_7] = {8, 9, 100, 988, 78, 9, 2};
        LineSensors<uint16_t , 1000, table_len_7> line(data, enable);
        LineDetector<uint16_t , table_len_7> detector(line);

        auto weights = detector.get_sensors_weights();

        std::array<int, table_len_7> reference_weight = {-3, -2, -1, 0, 1, 2, 3};
        EXPECT_EQ(reference_weight, weights);
    }

    {
        constexpr static size_t table_len_8 = 8;
        uint16_t data[table_len_8] = {8, 9, 100, 988, 78, 9, 2, 6};
        LineSensors<uint16_t , 1000, table_len_8> line(data, enable);
        LineDetector<uint16_t , table_len_8> detector(line);

        auto weights = detector.get_sensors_weights();

        std::array<int, table_len_8> reference_weight = {-4, -3, -2, -1, 1, 2, 3, 4};
        EXPECT_EQ(reference_weight, weights);
    }

}

TEST(LINE_DETECTOR, detect_line) {
    GPIOMock enable;
    constexpr static size_t table_len = 7;
    uint16_t data[table_len] = {8,9,100,988,78,9,2};
    LineSensors<uint16_t , 1000, table_len> line(data, enable);
    LineDetector<uint16_t , table_len> detector(line);

    float line_position =  detector.calculate_line_position();

    EXPECT_GT(line_position, -0.04f);
    EXPECT_GT(line_position, 0.0f);
}


enum class Direction {
    LEFT = false,
    RIGHT = true
};

void fill_data(uint16_t * data, int data_length, size_t main_index ) {
    for (int i = 0; i < data_length; i ++) {
        data[i] = 0;
    }

    data[main_index] = 1000;
}

TEST(LINE_DETECTOR, follow_line) {
    GPIOMock enable;
    constexpr static size_t table_len = 7;
    uint16_t data[table_len] = {0, 0, 0, 0, 0, 0, 0};
    LineSensors<uint16_t , 1000, table_len> line(data, enable);
    LineDetector<uint16_t , table_len> detector(line);

    Direction direction = Direction::LEFT;
    uint8_t index = 0;
    for (int i = 0; i < 1000; i++) {
        if ((i % (table_len - 1)) == 0) {
            if (direction == Direction::LEFT) {
                direction = Direction::RIGHT;
            } else {
                direction = Direction::LEFT;
            }
        }

        if (direction == Direction::RIGHT) {
            index++;
        } else {
            index--;
        }

        for (int j = 0; j < 4; j++) {
            fill_data(data, table_len, index);

//            std::cout << detector.calculate_line_position() << std::endl;
        }
    }
}

bool approximatelyEqual(float a, float b, float epsilon) {
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

TEST(LINE_DETECTOR, follow_line_float_value) {
    LineDetectorMOCK detector;

    Direction direction = Direction::LEFT;

    float line_position = 0.0;

    for (int i = 0; i < 1000; i++) {
        if (approximatelyEqual(line_position, static_cast<float>(detector.get_sensors_weights().at(0)), 0.01)) {
            direction = Direction::RIGHT;
        }

        if (approximatelyEqual(line_position, static_cast<float>(detector.get_sensors_weights().back()), 0.01)) {
            direction = Direction::LEFT;
        }


        if (approximatelyEqual(line_position, static_cast<float>(detector.get_sensors_weights().at(0)), 0.01)) {
            float last_line_position = line_position;

            line_position = -2;

            for (int j = 0; j < 30; j++) {

            }

            line_position = last_line_position;
        }



        if (direction == Direction::RIGHT) {
            line_position += 0.1;
        } else {
            line_position -= 0.1;
        }


        detector.update_line_position(line_position);
        detector.update_line_history();

//        for (auto item : detector.get_line_histiry()) {
//            std::cout << item << " ";
//        }
//        std::cout << std::endl;
    }
}

TEST(LINE_SENSORS, get_pointer_data) {
    GPIOMock enable;
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {4444,5555,6666,7123,8333,9234,2123,3432};
    uint8_t d1[table_len] = {17, 21, 26, 27, 32, 36, 8, 13};
    uint8_t d2[table_len] = {92, 179, 10, 211, 141, 18, 75, 104};
    LineSensors<uint16_t , 1000, table_len> line(data, enable);

    uint16_t* ret_data = line.get_data_pointer();

    for (unsigned short i : data) {
        EXPECT_EQ(i, *ret_data++);
    }

    volatile unsigned short * data1 = line.get_data_pointer();
    for (int i =0; i < line.size(); i ++) {
        uint16_t val = *data1++;
        EXPECT_EQ(d1[i], static_cast<uint8_t >(val >> 8u));
        EXPECT_EQ(d2[i], static_cast<uint8_t >(val >> 0u));
    }
}


