#define LARGE_JSON_BUFFERS 1
//#define LARGE_JSON_DOCUMENT_SIZE 8092
//#define SMALL_JSON_DOCUMENT_SIZE 2048

#include "adapter.h"
#include "Thing.h"
#include "WebThingAdapter.h"
#include "ledstrip.h"
#include "state.h"
#include "utils.h"
#include "modes.h"

namespace adapter {

    WebThingAdapter *adapter;
    const char* capabilities[] = {"Light", nullptr};
    ThingDevice device("LED-Strip", "LED-Strip", capabilities);
    ThingProperty on("on", "", BOOLEAN, nullptr);
    ThingProperty mode("mode", "", STRING, nullptr);
    ThingProperty colour("colour", "", STRING, nullptr);
    ThingProperty random("random", "", BOOLEAN, nullptr);
    ThingProperty speed("speed", "", INTEGER, nullptr);
    ThingProperty brightness("brightness", "", INTEGER, nullptr);

    void selectMode(uint8_t m) {
        colour.setValue(utils::colourint2string(state::modeState[m].colour).c_str());
        random.setValue({boolean: state::modeState[m].random});
        speed.setValue({integer: (float)(state::modeState[m].speed*100)});
        if (state::on) modes::init();
    }
    
    void begin() {
        adapter = new WebThingAdapter("ledstrip", WiFi.localIP());

        on.atType = "OnOffProperty";
        ThingPropertyValue onValue;
        onValue.boolean = state::on;
        on.setValue(onValue);

        mode.propertyEnum = modes::modes;
        ThingPropertyValue modeValue;
        modeValue.string = new String(modes::modes[state::mode]);
        mode.setValue(modeValue);

        colour.atType = "ColorProperty";
        ThingPropertyValue colourValue;
        colourValue.string = new String(utils::colourint2string(state::colour()).c_str());
        colour.setValue(colourValue);

        ThingPropertyValue randomValue;
        randomValue.boolean = state::random();
        random.setValue(randomValue);

        speed.atType = "LevelProperty";
        speed.title = "speed";
        speed.maximum = 100;
        speed.minimum = 0;
        speed.unit = "percent";
        ThingPropertyValue speedValue;
        speedValue.integer = state::speed();
        speed.setValue(speedValue);

        ThingPropertyValue brightnessValue;
        brightnessValue.integer = state::brightness;
        brightness.setValue(brightnessValue);
        brightness.atType = "BrightnessProperty";
        brightness.maximum = 100;
        brightness.minimum = 0;
        brightness.unit = "percent";

        device.addProperty(&on);
        device.addProperty(&mode);
        device.addProperty(&colour);
        device.addProperty(&random);
        device.addProperty(&speed);
        device.addProperty(&brightness);
        adapter->addDevice(&device);
        adapter->begin();

        Serial.println("HTTP server started");
        Serial.print("http://");
        Serial.print(WiFi.localIP());
        Serial.print("/things/");
        Serial.println(device.id);

        ledstrip::setBrightness(state::brightness);
        selectMode(state::mode);
    }

    void loop() {
        adapter->update();
    
        if (state::on != on.getValue().boolean) {
            state::on = on.getValue().boolean;
            Serial.print("on: ");
            Serial.println(state::on?"true":"false");
            state::save();
            modes::init();
        }

        if (state::brightness != brightness.getValue().integer) {
            state::brightness = brightness.getValue().integer;
            Serial.print("brightness: ");
            Serial.println(state::brightness);
            state::save();
            ledstrip::setBrightness(state::brightness);
        }

        uint8_t newmode = utils::indexOf(modes::modes, modes::numberModes, *(mode.getValue().string));
        if(newmode != state::mode) {
            Serial.print("mode: ");
            Serial.println(newmode);
            state::mode = newmode;
            state::save();
            selectMode(newmode);
        }

        uint32_t newcolour = utils::colourstring2int(*(colour.getValue().string));
        if(newcolour != state::colour()) {
            Serial.print("colour: ");
            Serial.println(newcolour, HEX);
            state::colour(newcolour);
            state::save();
            if (state::on) modes::init();
        }

        bool newrandom = random.getValue().boolean;
        if(newrandom != state::random()) {
            Serial.print("random: ");
            Serial.println(newrandom ? "true" : "false");
            state::random(newrandom);
            state::save();
            if (state::on) modes::init();
        }

        float newspeed = speed.getValue().integer/100.0;
        if(newspeed != state::speed()) {
            Serial.print("speed: ");
            Serial.println(newspeed);
            state::speed(newspeed);
            state::save();
            //if (state::on) modes::init();
        }

        if (state::on) {
            modes::step();
        } else {
            ledstrip::fill(0x000000);
            ledstrip::show();
        }
    }

}
