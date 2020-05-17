#pragma once

#include <Arduino.h>

namespace state {

    typedef struct {
        uint32_t colour;
        bool random;
        float speed;
    } ModeState;

    extern bool &on;
    extern float &brightness;
    extern uint8_t &mode;
    extern ModeState *modeState;

    void begin();
    void save();

    uint32_t colour();
    bool random();
    float speed();

    void colour(uint32_t);
    void random(bool);
    void speed(float);

}
