#define LARGE_JSON_BUFFERS 1
#define LARGE_JSON_DOCUMENT_SIZE 8092
#define SMALL_JSON_DOCUMENT_SIZE 2048

#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <stdlib.h>
#include <ctime>
#include <limits.h>

#include "Thing.h"
#include "WebThingAdapter.h"

#include <WiFiManager.h>
#include <Ticker.h>

WebThingAdapter *adapter;
const char* ledstripCapabilities[] = {"Light", nullptr};
ThingDevice ledstrip("LED-Strip", "LED-Strip", ledstripCapabilities);
ThingProperty on("on", "", BOOLEAN, nullptr);
ThingProperty mode("mode", "", STRING, nullptr);
ThingProperty colour("colour", "", STRING, nullptr);
ThingProperty rndm("random", "", BOOLEAN, nullptr);
ThingProperty speed("speed", "", INTEGER, nullptr);
ThingProperty brightness("brightness", "", INTEGER, nullptr);
const uint8_t modes_num = 13;
char * modes[modes_num+1] = {"Static", "Wipe", "Rainbow", "Rainbow Cycle", "Stars", "Flicker", "Fade", "Cycle", "Chase", "Blinking", "Alternate Blinking", "Smooth Blinking", "Smooth Alternate Blinking", nullptr};

#define PIN 4
#define LED_COUNT 114
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
#define DELAY 5

typedef struct {
  uint32_t colour;
  bool random;
  double speed;
} ModeConfig;
struct { 
  uint64_t initialized;
  bool on;
  uint8_t brightness;
  uint8_t mode;
  ModeConfig modeConfig[13];
} config;

WiFiManager wifiManager;
Ticker ticker;
bool tickOn = false;
void tick() {
  strip.fill(tickOn ? 0x000000 : 0xffffff, 0, strip.numPixels());
  strip.show();
  tickOn = !tickOn;
}
void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  ticker.attach(0.5, tick);
}

// Input a value 0 to 255 to get a colour value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int hexval(char c) {
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

int colourstring2int(String s){
  return hexval((s.charAt(6))<<0) + (hexval(s.charAt(5))<<4) + (hexval(s.charAt(4))<<8) + (hexval(s.charAt(3))<<12) + (hexval(s.charAt(2))<<16) + (hexval(s.charAt(1))<<20); 
}

String colourint2string(int s){
  return "#" + String((s>>20) & 16, HEX) + String((s>>16) & 16, HEX) + String((s>>12) & 16, HEX) + String((s>>8) & 16, HEX) + String((s>>4) & 16, HEX) + String((s>>0) & 16, HEX); 
}

uint8_t modestring2int(String mode) {
  for(int i = 0; i < modes_num; i++) {
    if(strcmp(mode.c_str(), modes[i]) == 0) {
      return i;
    }
  }
  return 0;
}

void saveConfig() {
  EEPROM.put(0, config);
  EEPROM.commit();  
}

void sleep(double ms) {
  if(ms <= 0)
    return;
  if (ms*1000 > 16383) {
    delay((int) ms);
  } else {
    delayMicroseconds((int) (ms*1000));
  }
}

int randCol() {
  return strip.ColorHSV(rand(), 255, 255);
  //return strip.Color(rand()%256, rand()%256, rand()%256);
}

int multCol(int col, double f) {
  return strip.Color((col>>16)%256 * f, (col>>8)%256 * f, col%256 * f);
}

int static_i;

int wipe_i;
int wipe_c;
unsigned int wipe_numcolours = 3;
int wipe_colours[] = {0xff0000, 0x00ff00, 0x0000ff};

int rainbow_j;

int rainbowcycle_j;

int stars_b;
int stars_p;
int stars_c;

int fade_i;
int fade_c;
int fade_lastc;

int cycle_c;
unsigned int cycle_numcolours = 3;
int cycle_colours[] = {0xff0000, 0x00ff00, 0x0000ff};

int chase_c;
unsigned int chase_numcolours = 3;
double chase_probability = 0.125;
int chase_colours[] = {0xff0000, 0x00ff00, 0x0000ff};

int blinking_i;
bool blinking_p;

int ablinking_i;
int ablinking_p;

int sblinking_i;
int sblinking_p;
int sblinking_c;

int sablinking_i;
int sablinking_p;
int sablinking_c[2];

void modeinit(int m, int col, bool rnd) {
  if(m == 1) { // Wipe
    //strip.fill(0x000000, 0, strip.numPixels());
    strip.show();
    wipe_i = 0;
    if (rnd) {
      wipe_c = randCol();
    } else {
      wipe_c = 0;
    }
  } else if(m == 2) { // Rainbow
    rainbow_j = 0;
  } else if(m == 3) { // Rainbow Cycle
    rainbowcycle_j = 0;
  } else if(m == 4) { // Stars
    stars_b = 0;
  } else if(m == 5) { // Flicker
  } else if(m == 6) { // Fade
    fade_i = 256;
    fade_lastc = randCol();
    strip.fill(fade_lastc, 0, strip.numPixels());
  } else if(m == 7) { // Cycle
    //strip.fill(0x000000, 0, strip.numPixels());
    cycle_c = 0;
  } else if(m == 8) { // Chase
    //strip.fill(0x000000, 0, strip.numPixels());
    chase_c = 0;
    if (rnd) {
      strip.setPixelColor(0, randCol());
    } else {
      cycle_c++;
      if (cycle_c >= cycle_numcolours) cycle_c = 0;
      strip.setPixelColor(0, cycle_colours[cycle_c]);
    }
  } else if(m == 9) { // Blinking
    blinking_i = 0;
    blinking_p = false;
  } else if(m == 10) { // Alternate Blinking
    ablinking_i = 0;
    ablinking_p = 0;
  } else if(m == 11) { // Smooth Blinking
    sblinking_i = 256;
    sblinking_p = 4;
  } else if(m == 12) { // Smooth Alternate Blinking
    sablinking_i = 0;
    sablinking_p = 0;
    if (rnd) {
      sablinking_c[0] = randCol();
      //sablinking_c[1] = randCol();
    }
  } else { // static
    strip.fill(col, 0, strip.numPixels());
    strip.show();
    static_i = 0;
  }
}

void modestep(int m, int col, bool rnd, double speed) {
  if (speed == 0)
    return;
  if (m == 1) { // Wipe
    if (rnd) {
      if (wipe_i >= strip.numPixels()) {
        wipe_i = 0;
        wipe_c = randCol();
      }
      strip.setPixelColor(wipe_i, wipe_c);
    } else {
      if (wipe_i >= strip.numPixels()) {
        wipe_i = 0;
        wipe_c++;
        if (wipe_c >= wipe_numcolours) wipe_c = 0;
      }
      strip.setPixelColor(wipe_i, wipe_colours[wipe_c]);
    }
    strip.show();
    sleep(1000*(1-speed));
    wipe_i++;
  } else if(m == 2) { // Rainbow
    if (rainbow_j >= 256) rainbow_j = 0;
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+rainbow_j) & 255));
    }
    strip.show();
    sleep(100*(1-speed));
    rainbow_j++;
  } else if(m == 3) { // Raindow Cycle
    if (rainbowcycle_j >= 256) rainbowcycle_j = 0;
    for(int i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + rainbowcycle_j) & 255));
    }
    strip.show();
    sleep(100*(1-speed));
    rainbowcycle_j++;
  } else if(m == 4) { // Stars
    if(stars_b < 0) {
      stars_b = 255;
      stars_p = rand()%strip.numPixels();
      stars_c = rnd ? randCol() : col;
    }
    double fac = stars_b / 255.0;
    strip.setPixelColor(stars_p, multCol(stars_c, fac));
    strip.show();
    sleep(10*(1-speed));
    stars_b--;
  } else if(m == 5) { // Flicker
    for(int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, randCol());
    }
    strip.show();
    sleep(1000 * (1-speed));
  } else if(m == 6) { // Fade
    if (rnd) {
      if(fade_i >= 256) {
        fade_i = 0;
        fade_lastc = fade_c;
        fade_c = randCol();
      }
      double fac = fade_i / 255.0;
      strip.fill(multCol(fade_lastc, 1-fac) + multCol(fade_c, fac), 0, strip.numPixels());
    } else {
      if(fade_i >= 256) fade_i = 0;
      strip.fill(Wheel(fade_i), 0, strip.numPixels());
    }
    strip.show();
    sleep(300*(1-speed));
    fade_i++;
  } else if(m == 7) { // Cycle
    for(int i = strip.numPixels()-1; i >= 1; i--){
      strip.setPixelColor(i, strip.getPixelColor(i-1));
    }
    if (rnd) {
      strip.setPixelColor(0, randCol());
    } else {
      cycle_c++;
      if (cycle_c >= cycle_numcolours) cycle_c = 0;
      strip.setPixelColor(0, cycle_colours[cycle_c]);
    }
    strip.show();
    sleep(1000*(1-speed));
  } else if(m == 8) { // Chase
    for(int i = strip.numPixels()-1; i >= 1; i--){
      strip.setPixelColor(i, strip.getPixelColor(i-1));
    }
    if((rand()*1.0)/INT_MAX < chase_probability) {
      if (rnd) {
        strip.setPixelColor(0, randCol());
      } else {
        cycle_c++;
        if (cycle_c >= cycle_numcolours) cycle_c = 0;
        strip.setPixelColor(0, cycle_colours[cycle_c]);
      }
    } else {
        strip.setPixelColor(0, 0x000000);
    }
    strip.show();
    sleep(1000*(1-speed));
  } else if(m == 9) { // Blinking
    if(blinking_i <= 0) {
      blinking_i = 100;
      blinking_p = !blinking_p;
      strip.fill(blinking_p ? 0x000000 : rnd ? randCol() : col, 0, strip.numPixels());
      strip.show();
    }
    sleep(50*(1-speed));
    blinking_i--;
  } else if(m == 10) { // Alternate Blinking
    if(ablinking_i <= 0) {
      ablinking_i = 100;
      ablinking_p = 1 - ablinking_p;
      int c = rnd ? randCol() : col;
      for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, i%2 == ablinking_p ? c : 0x000000);
      }
      strip.show();
    }
    sleep(50*(1-speed));
    ablinking_i--;
  } else if(m == 11) { // Smooth Blinking
    if(sblinking_i >= 256) {
      sblinking_i = 0;
      sblinking_p++;
      if (sblinking_p >= 4) {
        sblinking_p = 0;
        if (rnd) sblinking_c = randCol();
      }
    }
    if (sblinking_p == 0)
      strip.fill(multCol(rnd ? sblinking_c : col, sblinking_i/255.0), 0, strip.numPixels());
    else if (sblinking_p == 2)
      strip.fill(multCol(rnd ? sblinking_c : col, 1-sblinking_i/255.0), 0, strip.numPixels());
    strip.show();
    sleep(20*(1-speed));
    sblinking_i++;
  } else if(m == 12) { // Smooth Alternate Blinking
    if(sablinking_i <= 0) {
      sablinking_i = 255;
      sablinking_p++;
      if (sablinking_p >= 2) {
        sablinking_p = 0;
      }
      if (sablinking_p == 0 || sablinking_p == 1)
        if (rnd) sablinking_c[(sablinking_p+1)%2] = randCol();
    }
    for(int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, i%2 == sablinking_p ? multCol(rnd ? sablinking_c[i%2] : col, sablinking_i/255.0) : multCol(rnd ? sablinking_c[i%2] : col, 1-sablinking_i/255.0));
    }
    strip.show();
    sleep(20*(1-speed));
    sablinking_i--;
  } else { // Static
    if (rnd) {
      if(static_i == 0) {
        static_i = 100;
        strip.fill(randCol(), 0, strip.numPixels());
        strip.show();
      }
      static_i--;
      sleep(200*(1-speed));
    }
  }
}

void setup() {
  //setup Serial
  Serial.begin(9600);

  //setup EEPROM and load config
  EEPROM.begin(sizeof(config));
  EEPROM.get(0, config);
  if (config.initialized != 0x7a5b89f04e651c6f) {
    config.initialized = 0x7a5b89f04e651c6f;
    config.on = true;
    config.brightness = 100;
    config.mode = 0;
    config.modeConfig[0] = {0xff0000, false, 0.9};
    config.modeConfig[1] = {0x000000, false, 0.9};
    config.modeConfig[2] = {0x000000, false, 0.9};
    config.modeConfig[3] = {0x000000, false, 0.9};
    config.modeConfig[4] = {0xffffff, false, 0.9};
    config.modeConfig[5] = {0x000000, false, 0.9};
    config.modeConfig[6] = {0x000000, false, 0.9};
    config.modeConfig[7] = {0x000000, false, 0.9};
    config.modeConfig[8] = {0x000000, false, 0.9};
    config.modeConfig[9] = {0xffffff, false, 0.9};
    config.modeConfig[10] = {0xffffff, false, 0.9};
    config.modeConfig[11] = {0xffffff, false, 0.9};
    config.modeConfig[12] = {0xffffff, false, 0.9};
    saveConfig();
  }

  //setup LED-strip
  strip.begin();
  strip.show();
  Serial.println("LED-Strip setup");

  //execute first tick 
  tick();

  //connect WiFi
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("LED-Strip");
  ticker.detach();

  //setup WebThing Adapter
  adapter = new WebThingAdapter("ledstrip", WiFi.localIP());
  ThingPropertyValue onValue;
  onValue.boolean = config.on;
  on.setValue(onValue);
  on.atType = "OnOffProperty";
  ThingPropertyValue modeValue;
  modeValue.string = new String(modes[config.mode]);
  mode.setValue(modeValue);
  mode.propertyEnum = modes;
  ThingPropertyValue colourValue;
  colourValue.string = new String(colourint2string(config.modeConfig[config.mode].colour).c_str());
  colour.setValue(colourValue);
  colour.atType = "ColorProperty";
  ThingPropertyValue randomValue;
  randomValue.boolean = config.modeConfig[config.mode].random;
  rndm.setValue(randomValue);
  ThingPropertyValue speedValue;
  speedValue.integer = config.modeConfig[config.mode].speed;
  speed.setValue(speedValue);
  speed.atType = "LevelProperty";
  speed.title = "speed";
  speed.maximum = 100;
  speed.minimum = 0;
  speed.unit = "percent";
  ThingPropertyValue brightnessValue;
  brightnessValue.integer = config.brightness;
  brightness.setValue(brightnessValue);
  brightness.atType = "BrightnessProperty";
  brightness.maximum = 100;
  brightness.minimum = 0;
  brightness.unit = "percent";
  ledstrip.addProperty(&on);
  ledstrip.addProperty(&mode);
  ledstrip.addProperty(&colour);
  ledstrip.addProperty(&rndm);
  ledstrip.addProperty(&speed);
  ledstrip.addProperty(&brightness);
  adapter->addDevice(&ledstrip);
  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(ledstrip.id);

  //clear 
  strip.fill(0x000000, 0, strip.numPixels());
  strip.show();

}

void loop() {
  //Serial.println(WiFi.status());
  sleep(DELAY);

  adapter->update();
  
  bool forceinit = false;
  if (config.on != on.getValue().boolean) {
    config.on = on.getValue().boolean;
    Serial.print("on: ");
    Serial.println(config.on?"true":"false");
    saveConfig();
    forceinit = true;
  }
  
  if (config.brightness != brightness.getValue().integer) {
    config.brightness = brightness.getValue().integer;
    Serial.print("brightness: ");
    Serial.println(config.brightness);
    saveConfig();
    strip.setBrightness(config.brightness*2.54+1);
    strip.show();
  }

  if (on.getValue().boolean) {
    int newmode = modestring2int(*(mode.getValue().string));
    int newcolour = colourstring2int(*(colour.getValue().string));
    bool newrandom = rndm.getValue().boolean;
    double newspeed = speed.getValue().integer/100.0;
    if(newmode != config.mode || newcolour != config.modeConfig[newmode].colour || newrandom != config.modeConfig[newmode].random) {
      if (newmode != config.mode) {
        newcolour = config.modeConfig[newmode].colour;
        newrandom = config.modeConfig[newmode].random;
        newspeed = config.modeConfig[newmode].speed;
        colour.setValue(colourint2string(newcolour).c_str());
        rndm.setValue({boolean: config.modeConfig[newmode].random});
        speed.setValue({integer: (int)(config.modeConfig[newmode].speed*100)});
      }
      Serial.print("mode: ");
      Serial.println(newmode);
      Serial.print("colour: ");
      Serial.println(newcolour, HEX);
      Serial.print("random: ");
      Serial.println(newrandom?"true":"false");
      config.mode = newmode;
      config.modeConfig[newmode].colour = newcolour;
      config.modeConfig[newmode].random = newrandom;
      saveConfig();
      modeinit(newmode, newcolour, newrandom);
    }
    if(newspeed != config.modeConfig[newmode].speed) {
      Serial.print("speed: ");
      Serial.println(newspeed);
      config.modeConfig[newmode].speed = newspeed;
      saveConfig();
    }
    if (forceinit) modeinit(newmode, newcolour, newrandom);
    modestep(newmode, newcolour, newrandom, newspeed);
  } else {
    strip.fill(0x000000, 0, strip.numPixels());
    strip.show();
  }
  
}