#pragma once

#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_static {

    char* title = "Static";
    state::ModeState defaultState {colour: 0xff0000, random: false, speed: 0.9};
    uint8_t i;

    void init() {
        ledstrip::fill(state::colour());
        ledstrip::show();
        i = 0;
    }

    void step() {
        if (state::random()) {
            if(i == 0) {
                ledstrip::fill(utils::randCol());
                ledstrip::show();
            }
            i--;
            utils::sleep(200*(1-state::speed()));
        }
    }
}
