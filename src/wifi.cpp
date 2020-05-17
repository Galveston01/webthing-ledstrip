#include "wifi.h"
#include <WiFiManager.h>
#include <Ticker.h>
#include "ledstrip.h"

namespace wifi {
    WiFiManager wifiManager;
    Ticker ticker;

    void tick() {
        static bool tickOn = false;
        ledstrip::fill(tickOn ? 0x000000 : 0xffffff, 0, ledstrip::numPixels);
        ledstrip::show();
        tickOn = !tickOn;
    }

    void configModeCallback(WiFiManager *myWiFiManager) {
        Serial.println("Entered config mode");
        ticker.attach(0.5, tick);
    }

    void begin() {
        tick();
        wifiManager.setAPCallback(configModeCallback);
        wifiManager.autoConnect("LED-Strip");
        ticker.detach();
    }

}