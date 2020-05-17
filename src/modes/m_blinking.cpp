#include "m_static.h"
#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_blinking {
    
    uint8_t i;

    void init() {
        i = 0;
    }

    void step() {
        if((i&0b01111111) == 0) {
            ledstrip::fill((i&0b10000000) ? 0x000000 : (state::random() ? utils::randCol() : state::colour()));
            ledstrip::show();
        }
        utils::sleep(50*(1-state::speed()));
        i++;
    }
}
