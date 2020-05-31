#include "state.h"

#include <Arduino.h>
#include <EEPROM.h>
#include "modes.h"

namespace state {

    struct { 
        uint64_t initialized;
        bool on;
        float brightness;
        uint8_t mode;
        ModeState modeState[modes::numberModes];
    } s;

    bool &on = s.on;
    float &brightness = s.brightness;
    uint8_t &mode = s.mode;
    ModeState *modeState = s.modeState;

    void begin() {
        EEPROM.begin(sizeof(s));
        EEPROM.get(0, s);
        if (s.initialized != 0x7a5b89f04e651c6e) {
            Serial.println("Initializing State");
            s.initialized = 0x7a5b89f04e651c6e;
            s.on = true;
            s.brightness = 100;
            s.mode = 0;
            for (uint8_t i = 0; i < modes::numberModes; i++)
                modeState[i] = modes::defaultState(i);
            save();
        }
        
        /*Serial.println("Loading State");
        Serial.println("on: "+String(s.on?1:0));
        Serial.println("brightness: "+String(s.brightness));
        Serial.println("mode: "+String(s.mode));
        for(int i = 0; i < modes::numberModes; i++) {
            Serial.println("=== mode "+String(i)+" ===");
            Serial.println("colour: "+String(s.modeState[i].colour, HEX));
            Serial.println("random: "+String(s.modeState[i].random?1:0));
            Serial.println("speed: "+String(s.modeState[i].speed));
        }*/
    }

    void save() {
        EEPROM.put(0, s);
        EEPROM.commit();  
    }

    uint32_t colour() {
        return modeState[mode].colour;
    }

    bool random() {
        return modeState[mode].random;
    }

    float speed() {
        return modeState[mode].speed;
    }

    void colour(uint32_t c) {
        modeState[mode].colour = c;
    }

    void random(bool r) {
        modeState[mode].random = r;
    }

    void speed(float s) {
        modeState[mode].speed = s;
    }

}