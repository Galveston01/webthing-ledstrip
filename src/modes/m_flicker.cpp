#include "m_static.h"
#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_flicker {

    void init() {
    }

    void step() {
        for(uint8_t i = 0; i < ledstrip::numPixels; i++) {
            ledstrip::setPixelColor(i, utils::randCol());
        }
        ledstrip::show();
        utils::sleep(1000 * (1-state::speed()));
    }
}
