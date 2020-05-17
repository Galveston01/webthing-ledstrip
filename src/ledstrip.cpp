#include "ledstrip.h"
#include <Adafruit_NeoPixel.h>
#include "state.h"
#include "modes/m_static.h"
#include "modes/m_wipe.h"
#include "modes/m_rainbow.h"
#include "modes/m_rainbowcycle.h"
#include "modes/m_stars.h"
#include "modes/m_flicker.h"
#include "modes/m_fade.h"
#include "modes/m_cycle.h"
#include "modes/m_chase.h"
#include "modes/m_blinking.h"
#include "modes/m_alternateblinking.h"
#include "modes/m_smoothblinking.h"
#include "modes/m_smoothalternateblinking.h"

namespace ledstrip {

    const uint8_t pin = 4;
    const uint16_t numPixels = 114;
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);

    char* modes[] = {
        "Static", 
        "Wipe", 
        "Rainbow", 
        "Rainbow Cycle", 
        "Stars", 
        "Flicker", 
        "Fade", 
        "Cycle", 
        "Chase", 
        "Blinking", 
        "Alternate Blinking", 
        "Smooth Blinking", 
        "Smooth Alternate Blinking", 
        nullptr
    };

    void (*init_functions[])() = {
        m_static::init,
        m_wipe::init,
        m_rainbow::init,
        m_rainbowcycle::init,
        m_stars::init,
        m_flicker::init,
        m_fade::init,
        m_cycle::init,
        m_chase::init,
        m_blinking::init,
        m_alternateblinking::init,
        m_smoothblinking::init,
        m_smoothalternateblinking::init,
    };

    void (*step_functions[])() = {
        m_static::step,
        m_wipe::step,
        m_rainbow::step,
        m_rainbowcycle::step,
        m_stars::step,
        m_flicker::step,
        m_fade::step,
        m_cycle::step,
        m_chase::step,
        m_blinking::step,
        m_alternateblinking::step,
        m_smoothblinking::step,
        m_smoothalternateblinking::step,
    };

    void begin() {
        strip.begin();
        show();
    }

    void setPixelColor(uint16_t n, uint32_t col) {
        strip.setPixelColor(n, col);
    }

    uint32_t getPixelColor(uint16_t n) {
        return strip.getPixelColor(n);
    }

    void fill(uint32_t col, uint16_t a, uint16_t b) {
        strip.fill(col, a, b);
    }

    void fill(uint32_t col) {
        strip.fill(col, 0, numPixels);
    }
        
    void show() {
        strip.show();
    }

    void setBrightness(float b) {
        strip.setBrightness(b*2.54+1);
        strip.show();
    }

    void modeinit() {
        init_functions[state::mode]();
    }

    void modestep() {
        if(state::speed() != 0)
            step_functions[state::mode]();
    }

}