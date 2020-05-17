#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_rainbowcycle {

    char* title = "Rainbow Cycle";
    state::ModeState defaultState {colour: 0x000000, random: false, speed: 0.9};
    uint8_t j;

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
        j = 0;
    }

    void step() {
        for(uint16_t i = 0; i < ledstrip::numPixels; i++) {
            ledstrip::setPixelColor(i, Wheel(((i * 256 / ledstrip::numPixels) + j) & 255));
        }
        ledstrip::show();
        utils::sleep(100*(1-state::speed()));
        j++;
    }
}