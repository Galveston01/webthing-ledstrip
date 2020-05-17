#pragma once

#include <Arduino.h>

namespace ledstrip {

    const uint8_t numberModes = 13;
    extern char* modes[numberModes+1];
    extern const uint16_t numPixels;

    void begin();
    void setPixelColor (uint16_t, uint32_t);
    uint32_t getPixelColor (uint16_t);
    void fill (uint32_t, uint16_t, uint16_t);
    void fill (uint32_t);
    void show();
    void setBrightness(float);
    void modeinit();
    void modestep();
} 
