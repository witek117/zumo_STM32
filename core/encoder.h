#ifndef ZUMO_ENCODER_H
#define ZUMO_ENCODER_H

#include "LineDebounce.h"
#include <hal.h>

class Encoder {
protected:
    hal::GPIO &Pin_P;
    hal::GPIO &Pin_N;

    int encoder_prescaler = 1;
    constexpr static int encoder_debounce_stable = 1;

    volatile int encoderCount = 0;

    struct State {
        bool p, n;
    };

public:
    Encoder(hal::GPIO &P, hal::GPIO &N, int encoder_prescaler = 1) : Pin_P(P), Pin_N(N), encoder_prescaler(encoder_prescaler) {

    }

    int encoderGetCountAndReset() {

#ifdef __arm__
//        __disable_irq();
#endif
        int toReturn = encoderCount / encoder_prescaler;
        if (toReturn != 0) {
            encoderCount = 0;
        }
#ifdef __arm__
//        __enable_irq();
#endif
        return toReturn;
    }

    libs::debouncer::LineDebounce<encoder_debounce_stable> EncoderDebouncedLineP;
    libs::debouncer::LineDebounce<encoder_debounce_stable> EncoderDebouncedLineN;

    void encoder10kHzTickISR() {
        State now;
        now.p = Pin_P.get();
        now.n = Pin_N.get();
        EncoderDebouncedLineP.tick(now.p);
        EncoderDebouncedLineN.tick(now.n);

        if (EncoderDebouncedLineP.getState() == libs::debouncer::State::TRANSITION &&
            EncoderDebouncedLineN.getState() == libs::debouncer::State::STABLE) {
            if (EncoderDebouncedLineP.getValue() == EncoderDebouncedLineN.getValue()) {
                encoderCount++;
            } else {
                encoderCount--;
            }
        } else if (EncoderDebouncedLineP.getState() == libs::debouncer::State::STABLE &&
                   EncoderDebouncedLineN.getState() == libs::debouncer::State::TRANSITION) {
            if (EncoderDebouncedLineP.getValue() == EncoderDebouncedLineN.getValue()) {
                encoderCount--;
            } else {
                encoderCount++;
            }
        }
    }
};

class Encoder_with_button : public Encoder {
    hal::GPIO &Button;

    constexpr static int button_debounce_stable = 20;
    void (*pressed_handler)() = nullptr;
    void (*released_handler)() = nullptr;

public:

    Encoder_with_button(hal::GPIO &P, hal::GPIO &N, int encoder_prescaler, hal::GPIO &Button, void (*pressed_handler)(), void (*released_handler)()):
        Encoder(P, N, encoder_prescaler),
        Button(Button),
        pressed_handler(pressed_handler),
        released_handler(released_handler) {

    }

    libs::debouncer::LineDebounce<button_debounce_stable> ButtonDebouncedLine;

    void encoder10kHzTickISR() {
        Encoder::encoder10kHzTickISR();

        bool btn = Button.get();

        ButtonDebouncedLine.tick(btn);

        if (ButtonDebouncedLine.getState() == libs::debouncer::State::TRANSITION && !ButtonDebouncedLine.getValue()) {
            if (pressed_handler) {
                pressed_handler();
            }
        }

        if (ButtonDebouncedLine.getState() == libs::debouncer::State::TRANSITION && ButtonDebouncedLine.getValue()) {
            if (released_handler) {
                released_handler();
            }
        }
    }
};


#endif //ZUMO_ENCODER_H
