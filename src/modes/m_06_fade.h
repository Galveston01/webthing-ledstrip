#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_fade {

    char* title = "Fade";
    state::ModeState defaultState {colour: 0x000000, random: false, speed: 0.9};
    uint8_t i;
    uint32_t c;
    uint32_t lastc;

    uint32_t Wheel(uint8_t WheelPos) {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85) {
            return utils::colourrgb2int(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        if(WheelPos < 170) {
            WheelPos -= 85;
            return utils::colourrgb2int(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        WheelPos -= 170;
        return utils::colourrgb2int(WheelPos * 3, 255 - WheelPos * 3, 0);
    }

    void init() {
        i = 255;
        c = utils::randCol();
        ledstrip::fill(lastc);
    }

    void step() {
        if (state::random()) {
            if(i == 0) {
                lastc = c;
                c = utils::randCol();
            }
            float fac = i / 255.0;
            ledstrip::fill(utils::multCol(lastc, 1-fac) + utils::multCol(c, fac));
        } else {
            ledstrip::fill(Wheel(i));
        }
        ledstrip::show();
        utils::sleep(300*(1-state::speed()));
        i++;
    }
}