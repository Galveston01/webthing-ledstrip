#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_smoothblinking {
    
    char* title = "Smooth Blinking";
    state::ModeState defaultState {colour: 0xffffff, random: false, speed: 0.9};
    uint8_t i;
    uint8_t p;
    uint32_t c;

    void init() {
        i = 0;
        p = 3;
    }

    void step() {
        if(i == 0) {
            p++;
            if (p >= 3) {
                p = 0;
                if (state::random()) c = utils::randCol();
            }
        }
        if (p == 0)
            ledstrip::fill(utils::multCol(state::random() ? c : state::colour(), i/255.0));
        else if (p == 1)
            ledstrip::fill(utils::multCol(state::random() ? c : state::colour(), 1-i/255.0));
        ledstrip::show();
        utils::sleep(20*(1-state::speed()));
        i++;
    }
}