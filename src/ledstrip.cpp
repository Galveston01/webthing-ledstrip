#include "ledstrip.h"
#include <Adafruit_NeoPixel.h>
#include "state.h"
#include "modes.h"

namespace ledstrip {

    const uint8_t pin = 4;
    const uint16_t numPixels = 114;
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);

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

}