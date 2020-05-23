#include <gtest/gtest.h>
#include "cores/mean.h"
#include <iostream>
#include <array>
#include <gmock/gmock-matchers.h>

TEST(MEAN, zeros) {
    Mean <int, 20>basic_data;

    ASSERT_THAT(basic_data.get_all_data(),
                ::testing::ElementsAreArray({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
}

TEST(MEAN, basic) {
    Mean <int, 20>basic_data;
    std::array<int, 20> expects = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

    for (int i =0; i < 20; i ++) {
        basic_data.put_value(i);
        EXPECT_EQ(basic_data.get_value(), expects.at(i));
    }

    ASSERT_THAT(basic_data.get_all_data(),
                ::testing::ElementsAreArray({ 19, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 }));
}

TEST(MEAN, basic_with_divider) {
    GTEST_SKIP();

    Mean<int, 20> basic_data;

    for (int i =0; i < 20; i ++) {
        basic_data.put_value(i, 2);
    }

    for (auto item : basic_data.get_all_data() ) {
        std::cout <<  item << " " << std::endl;
    }

//    ASSERT_THAT(basic_data.get_all_data(),
//                ::testing::ElementsAreArray({ 0, 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
}