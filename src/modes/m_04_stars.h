#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_stars {

    char* title = "Stars";
    state::ModeState defaultState {colour: 0xffff00, random: false, speed: 0.9};
    uint8_t b;
    uint8_t p;
    uint32_t c;
    bool clearOnStartup = false;

    void init() {
        if(clearOnStartup) {
            ledstrip::fill(0x000000);
            ledstrip::show();
        }
        b = 0;
    }

    void step() {
        if(b == 255) {
            p = rand()%ledstrip::numPixels;
            c = state::random() ? utils::randCol() : state::colour();
        }
        float fac = b / 255.0;
        ledstrip::setPixelColor(p, utils::multCol(c, fac));
        ledstrip::show();
        utils::sleep(10*(1-state::speed()));
        b--;
    }
} 
