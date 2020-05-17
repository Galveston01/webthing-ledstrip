#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_smoothalternateblinking {

    char* title = "Smooth Alternate Blinking";
    state::ModeState defaultState {colour: 0xffffff, random: false, speed: 0.9};
    uint8_t i;
    uint8_t p;
    uint32_t c[2];

    void init() {
        i = 0;
        p = 0;
        if (state::random()) {
            c[0] = utils::randCol();
        }
    }

    void step() {
        if(i == 0) {
            p = (p+1)%2;
            if (p == 0 || p == 1)
                if (state::random()) c[p%2] = utils::randCol();
        }
        for(int j = 0; j < ledstrip::numPixels; j++) {
            ledstrip::setPixelColor(j, 
                j%2 == p ? 
                utils::multCol(state::random() ? c[j%2] : state::colour(), i/255.0) : 
                utils::multCol(state::random() ? c[j%2] : state::colour(), 1-i/255.0)
            );
        }
        ledstrip::show();
        utils::sleep(20*(1-state::speed()));
        i++;
    }
}