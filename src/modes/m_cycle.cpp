#include "m_static.h"
#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"

namespace m_cycle {
    uint32_t c;
    uint8_t numcolours = 3;
    uint32_t colours[] = {0xff0000, 0x00ff00, 0x0000ff};
    bool clearOnStartup = false;

    void init() {
        if(clearOnStartup) {
            ledstrip::fill(0x000000);
            ledstrip::show();
        }
        c = 0;
    }

    void step() {
        for(uint8_t i = ledstrip::numPixels-1; i >= 1; i--){
            ledstrip::setPixelColor(i, ledstrip::getPixelColor(i-1));
        }
        if (state::random()) {
            ledstrip::setPixelColor(0, utils::randCol());
        } else {
            c++;
        if (c >= numcolours) c = 0;
            ledstrip::setPixelColor(0, colours[c]);
        }
        ledstrip::show();
        utils::sleep(1000*(1-state::speed()));
    }
}
