#include "m_static.h"
#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_alternateblinking {

    uint8_t i;   

    void init() {
        i = 0;
    }

    void step() {
        if((i&0b01111111) == 0) {
            uint32_t c = state::random() ? utils::randCol() : state::colour();
            for(uint8_t j = 0; j < ledstrip::numPixels; j++) {
                ledstrip::setPixelColor(j, j%2 == ((i&0b10000000)>>7) ? c : 0x000000);
            }
            ledstrip::show();
        }
        utils::sleep(50*(1-state::speed()));
        i++;
    }
}
