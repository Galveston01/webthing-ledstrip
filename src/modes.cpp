#include "state.h"

#include "modes/m_00_static.h"
#include "modes/m_01_wipe.h"
#include "modes/m_02_rainbow.h"
#include "modes/m_03_rainbowcycle.h"
#include "modes/m_04_stars.h"
#include "modes/m_05_flicker.h"
#include "modes/m_06_fade.h"
#include "modes/m_07_cycle.h"
#include "modes/m_08_chase.h"
#include "modes/m_09_blinking.h"
#include "modes/m_10_alternateblinking.h"
#include "modes/m_11_smoothblinking.h"
#include "modes/m_12_smoothalternateblinking.h"
namespace modes {

    char* modes[] = {
        m_static::title, 
        m_wipe::title, 
        m_rainbow::title, 
        m_rainbowcycle::title, 
        m_stars::title, 
        m_flicker::title, 
        m_fade::title, 
        m_cycle::title, 
        m_chase::title, 
        m_blinking::title, 
        m_alternateblinking::title, 
        m_smoothblinking::title, 
        m_smoothalternateblinking::title, 
    };

    void init() {
        switch(state::mode) {
            case 0: m_static::init(); break;
            case 1: m_wipe::init(); break;
            case 2: m_rainbow::init(); break;
            case 3: m_rainbowcycle::init(); break;
            case 4: m_stars::init(); break;
            case 5: m_flicker::init(); break;
            case 6: m_fade::init(); break;
            case 7: m_cycle::init(); break;
            case 8: m_chase::init(); break;
            case 9: m_blinking::init(); break;
            case 10: m_alternateblinking::init(); break;
            case 11: m_smoothblinking::init(); break;
            case 12: m_smoothalternateblinking::init(); break;
        }
    }

    void step() {
        switch(state::mode) {
            case 0: m_static::step(); break;
            case 1: m_wipe::step(); break;
            case 2: m_rainbow::step(); break;
            case 3: m_rainbowcycle::step(); break;
            case 4: m_stars::step(); break;
            case 5: m_flicker::step(); break;
            case 6: m_fade::step(); break;
            case 7: m_cycle::step(); break;
            case 8: m_chase::step(); break;
            case 9: m_blinking::step(); break;
            case 10: m_alternateblinking::step(); break;
            case 11: m_smoothblinking::step(); break;
            case 12: m_smoothalternateblinking::step(); break;
        }
    }

    state::ModeState defaultState(uint8_t mode) {
        switch(mode) {
            case 0: return m_static::defaultState;
            case 1: return m_wipe::defaultState;
            case 2: return m_rainbow::defaultState;
            case 3: return m_rainbowcycle::defaultState;
            case 4: return m_stars::defaultState;
            case 5: return m_flicker::defaultState;
            case 6: return m_fade::defaultState;
            case 7: return m_cycle::defaultState;
            case 8: return m_chase::defaultState;
            case 9: return m_blinking::defaultState;
            case 10: return m_alternateblinking::defaultState;
            case 11: return m_smoothblinking::defaultState;
            case 12: return m_smoothalternateblinking::defaultState;
        }
    }

};
