#include <gtest/gtest.h>
#include <tuple>
#include "KTIR0711S.h"

TEST(KTIR0711S, sensor) {
    uint16_t data = 4;
    KTIR0711S<decltype(data), 1000> sensor(data);

    EXPECT_EQ(sensor.get_raw_value(), 4);
    data = 100;
    EXPECT_EQ(sensor.get_raw_value(), 100);
    EXPECT_EQ(sensor.get_normalized_value(), 0.1f);
    data = 1200;
    EXPECT_EQ(sensor.get_normalized_value(), 1);
}

TEST(KTIR0711S, sensors_line) {
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {4,5,6,7,8,9,2,3};
    LineSensors<uint16_t , 1000, table_len> line(data);

    constexpr static float max_value = 500.0;

    for(size_t i = 0; i < table_len; i++) {
        EXPECT_EQ(data[i], line.get_single_raw_value(i));
        EXPECT_EQ((float)data[i] / 1000.0f, line.get_single_normalized_value(i));
        EXPECT_EQ((float)data[i] / max_value, line.get_single_normalized_value(i, max_value));
    }
}



TEST(KTIR0711S, sensors_line_too_big_index) {
    constexpr static size_t table_len = 8;
    uint16_t data[table_len] = {4,5,6,7,8,9,2,3};
    LineSensors<uint16_t , 1000, table_len> line(data);

    constexpr static float max_value = 500.0;

    for(size_t i = table_len; i < (table_len + 3); i++) {
        EXPECT_EQ(0, line.get_single_raw_value(i));
        EXPECT_EQ(0, line.get_single_normalized_value(i));
        EXPECT_EQ(0, line.get_single_normalized_value(i, max_value));
    }
}

TEST(LINE_DETECTOR, weights) {
    constexpr static size_t table_len = 7;
    uint16_t data[table_len] = {8,9,100,988,78,9,2};
    LineSensors<uint16_t , 1000, table_len> line(data);
    LineDetector<uint16_t , table_len> detector(line);

    float line_postion =  detector.calculate_line_position();
    std::cout << line_postion << std::endl;
}
