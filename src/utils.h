#pragma once

#include <Arduino.h>

namespace utils {
    uint32_t colourstring2int(String);
    String colourint2string(uint32_t);
    uint8_t indexOf(char*[], uint8_t, String);
    void sleep(float);
    uint32_t colourrgb2int(uint8_t, uint8_t, uint8_t);
    uint32_t colourhsv2int(uint8_t, uint8_t, uint8_t);
    uint32_t randCol();
    uint32_t multCol(uint32_t, float);
}