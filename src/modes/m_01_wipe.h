#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_wipe {

    char* title = "Wipe";
    state::ModeState defaultState {colour: 0x000000, random: false, speed: 0.9};
    uint8_t i;
    uint32_t c;
    uint8_t numcolours = 3;
    uint32_t colours[] = {0xff0000, 0x00ff00, 0x0000ff};
    bool clearOnStartup = false;

    void init() {
        if(clearOnStartup) {
            ledstrip::fill(0x000000);
            ledstrip::show();
        }
        i = 0;
        if (state::random()) {
            c = utils::randCol();
        } else {
            c = 0;
        }
    }

    void step() {
        if(state::random()) {
            if (i >= ledstrip::numPixels) {
                i = 0;
                c = utils::randCol();
            }
            ledstrip::setPixelColor(i, c);
        } else {
            if (i >= ledstrip::numPixels) {
                i = 0;
                c++;
                if (c >= numcolours) c = 0;
            }
            ledstrip::setPixelColor(i, colours[c]);
        }
        ledstrip::show();
        utils::sleep(1000*(1-state::speed()));
        i++;
    }
}
