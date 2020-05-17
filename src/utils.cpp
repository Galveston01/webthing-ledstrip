#include "utils.h"

namespace utils {

    uint8_t hexval(char c) {
        switch(c) {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'a': case 'A': return 10;
            case 'b': case 'B': return 11;
            case 'c': case 'C': return 12;
            case 'd': case 'D': return 13;
            case 'e': case 'E': return 14;
            case 'f': case 'F': return 15;
        }
    }

    uint32_t colourstring2int(String s){
        return hexval((s.charAt(6))<<0) + (hexval(s.charAt(5))<<4) + (hexval(s.charAt(4))<<8) + (hexval(s.charAt(3))<<12) + (hexval(s.charAt(2))<<16) + (hexval(s.charAt(1))<<20); 
    }

    String colourint2string(uint32_t s){
        return "#" + String((s>>20) & 15, HEX) + String((s>>16) & 15, HEX) + String((s>>12) & 15, HEX) + String((s>>8) & 15, HEX) + String((s>>4) & 15, HEX) + String((s>>0) & 15, HEX); 
    }

    uint8_t indexOf(char* arr[], uint8_t arr_length, String str) {
        for(uint8_t i = 0; i < arr_length; i++) {
        if(strcmp(str.c_str(), arr[i]) == 0) {
            return i;
        }
        }
        return 0;
    }

    void sleep(float ms) {
        if(ms <= 0)
            return;
        if (ms*1000 > 16383) {
            delay((uint16_t) ms);
        } else {
            delayMicroseconds((uint16_t) (ms*1000));
        }
    }

    uint32_t colourrgb2int(uint8_t r, uint8_t g, uint8_t b) {
        return (r<<16) + (g<<8) + b;
    }

    uint32_t colourhsv2int(uint8_t h, uint8_t s, uint8_t v) {
        unsigned char region, remainder, p, q, t;

        uint8_t r, g, b;

        if (s == 0)
        {
            r = v;
            g = v;
            b = v;
            return colourrgb2int(r, g, b);
        }

        region = h / 43;
        remainder = (h - (region * 43)) * 6; 

        p = (v * (255 - s)) >> 8;
        q = (v * (255 - ((s * remainder) >> 8))) >> 8;
        t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

        switch (region)
        {
            case 0:
                r = v; g = t; b = p;
                break;
            case 1:
                r = q; g = v; b = p;
                break;
            case 2:
                r = p; g = v; b = t;
                break;
            case 3:
                r = p; g = q; b = v;
                break;
            case 4:
                r = t; g = p; b = v;
                break;
            default:
                r = v; g = p; b = q;
                break;
        }

        return colourrgb2int(r, g, b);
    }

    uint32_t randCol() {
        return colourhsv2int(rand()%255, 255, 255);
        //return strip.Color(rand()%256, rand()%256, rand()%256);
    }

    uint32_t multCol(uint32_t col, float f) {
        return colourrgb2int((col>>16)%256 * f, (col>>8)%256 * f, col%256 * f);
    }

}