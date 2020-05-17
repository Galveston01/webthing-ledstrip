#pragma once

#include <Arduino.h>

namespace ledstrip {

    extern const uint16_t numPixels;

    void begin();
    void setPixelColor (uint16_t, uint32_t);
    uint32_t getPixelColor (uint16_t);
    void fill (uint32_t, uint16_t, uint16_t);
    void fill (uint32_t);
    void show();
    void setBrightness(float);
} 
