#include <Arduino.h>

#include "utils.h"
#include "ledstrip.h"
#include "adapter.h"
#include "wifi.h"
#include "state.h"

#define DELAY 5

void setup() {
  Serial.begin(9600);

  state::begin();
  ledstrip::begin();
  wifi::begin();
  adapter::begin();
}

void loop() {
  utils::sleep(DELAY);
  adapter::loop();
}