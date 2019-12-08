#include "gtest/gtest.h"
#include "MCP9700.h"

#include <iostream>

TEST(MCP9700, temperature_conversion) {
    uint16_t data = 923;
    uint16_t *data_pointer = &data;
    MCP9700<uint16_t > thermometer(*data_pointer, 4096, 3.3f);

    termometer.init();

    float returned_value = termometer.get_temperature();
    EXPECT_GE(returned_value, 24.36);
    EXPECT_LE(returned_value, 24.37);
}

TEST(MCP9700, temperature_initialise) {
    uint16_t data = 923;
    uint16_t *data_pointer = &data;
    MCP9700<uint16_t > thermometer(*data_pointer, 4096, 3.3f);
    EXPECT_EQ(thermometer.get_temperature(), 0.0f);

    termometer.init();

    float returned_value = termometer.get_temperature();
    EXPECT_GE(returned_value, 24.36);
    EXPECT_LE(returned_value, 24.37);

    termometer.deinit();
    EXPECT_EQ(thermometer.get_temperature(), 0.0f);
}