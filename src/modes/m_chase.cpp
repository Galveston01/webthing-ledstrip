#include "m_static.h"
#include "../utils.h"
#include "../state.h"
#include "../ledstrip.h"
#include <limits.h>

namespace m_chase {
    uint32_t c;
    uint8_t numcolours = 3;
    uint32_t colours[] = {0xff0000, 0x00ff00, 0x0000ff};
    float probability = 0.125;
    bool clearOnStartup = false;

    void init() {
        if(clearOnStartup) {
            ledstrip::fill(0x000000);
            ledstrip::show();
        }
        c = 0;
        if (state::random()) {
            ledstrip::setPixelColor(0, utils::randCol());
        } else {
            c++;
            if (c >= numcolours) c = 0;
            ledstrip::setPixelColor(0, colours[c]);
        }
    }

    void step() {
        for(uint8_t i = ledstrip::numPixels-1; i >= 1; i--){
            ledstrip::setPixelColor(i, ledstrip::getPixelColor(i-1));
        }
        if((rand()*1.0)/INT_MAX < probability) {
            if (state::random()) {
                ledstrip::setPixelColor(0, utils::randCol());
            } else {
                c++;
                if (c >= numcolours) c = 0;
                ledstrip::setPixelColor(0, colours[c]);
            }
        } else {
            ledstrip::setPixelColor(0, 0x000000);
        }
        ledstrip::show();
        utils::sleep(1000*(1-state::speed()));
    }
}
