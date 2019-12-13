#ifndef ZUMO_WS2812B_H
#define ZUMO_WS2812B_H



template <int size>
class WS2812B {
public:
    virtual void set_color(int index, uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual void send() = 0;
};


#endif //ZUMO_WS2812B_H
