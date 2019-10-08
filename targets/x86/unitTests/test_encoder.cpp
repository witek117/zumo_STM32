#include "gtest/gtest.h"
#include "encoder.h"
#include "GPIO/GPIO.h"
#include <gmock/gmock.h>
#include <iostream>
#include <vector>

using ::testing::DefaultValue;
using ::testing::Return;

class GPIOMock : public GPIO {
public:
    MOCK_METHOD(void, set, ());
    MOCK_METHOD(void, reset, ());
    MOCK_METHOD(void, setMode, ());
    MOCK_METHOD(bool, get, ());
};

TEST(Encoder, counter_forward_reverse) {
    GPIOMock P;
    GPIOMock N;

    Encoder encoder(P, N, 4);
    bool state_P = false;
    bool state_N = false;

    constexpr int number_of_same_steps = 7;

    std::vector<std::pair<bool, bool>> states;

    for (int i =0; i < 100; i ++) {

        if ((i % number_of_same_steps) == 0) {
            state_P = !state_P;
        }

        if (((i + 2) % number_of_same_steps ) == 0) {
            state_N = !state_N;
        }

        states.push_back({state_P, state_N});
    }

    for (auto state: states) {
        EXPECT_CALL(P, get()).Times(4).WillRepeatedly(Return(state.second));
        EXPECT_CALL(N, get()).Times(4).WillRepeatedly(Return(state.first));

        for (int j = 0; j < 4; j ++) {
            encoder.encoder10kHzTickISR();
        }
    }

    EXPECT_EQ(7, encoder.encoderGetCountAndReset());
    EXPECT_EQ(0, encoder.encoderGetCountAndReset());

    for (auto state: states) {
        EXPECT_CALL(P, get()).Times(4).WillRepeatedly(Return(state.first));
        EXPECT_CALL(N, get()).Times(4).WillRepeatedly(Return(state.second));

        for (int j = 0; j < 4; j ++) {
            encoder.encoder10kHzTickISR();
        }
    }

    EXPECT_EQ(-7, encoder.encoderGetCountAndReset() );
    EXPECT_EQ(0, encoder.encoderGetCountAndReset() );
}


TEST(Encoder, only_noise) {
    GPIOMock P;
    GPIOMock N;

    Encoder encoder(P, N, 4);
    bool state = false;
    std::vector<std::pair<bool, bool>> states;

    for (int i = 0; i < 100; i++) {
        state = !state;

        states.push_back({state, !state});
    }

    for (auto state: states) {
        EXPECT_CALL(P, get()).WillRepeatedly(Return(state.first));
        EXPECT_CALL(N, get()).WillRepeatedly(Return(state.second));

        encoder.encoder10kHzTickISR();
    }
    EXPECT_EQ(0, encoder.encoderGetCountAndReset() );

    for (auto state: states) {
        EXPECT_CALL(P, get()).WillRepeatedly(Return(state.first));
        EXPECT_CALL(N, get()).WillRepeatedly(Return(state.second));

        for (int i = 0; i < 2; i++) {
            encoder.encoder10kHzTickISR();
        }
    }
    EXPECT_EQ(0, encoder.encoderGetCountAndReset() );

    for (auto state: states) {
        EXPECT_CALL(P, get()).Times(6).WillRepeatedly(Return(state.first));
        EXPECT_CALL(N, get()).Times(6).WillRepeatedly(Return(state.second));

        for (int i =0; i < 6; i ++) {
            encoder.encoder10kHzTickISR();
        }
    }
    EXPECT_EQ(0, encoder.encoderGetCountAndReset() );
}

