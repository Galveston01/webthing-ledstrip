#pragma once

#include <Arduino.h>
#include "state.h"

namespace modes {
    const int numberModes = 13;
    extern char* modes[];
    void init();
    void step();
    state::ModeState defaultState(uint8_t);
};
