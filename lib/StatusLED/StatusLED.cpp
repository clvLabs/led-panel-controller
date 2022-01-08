#include <Arduino.h>
#include "StatusLED.h"

StatusLED::StatusLED() {}

StatusLED::~StatusLED() {}

void StatusLED::start() {
  mLED.start(LED_BUILTIN, false);
}

void StatusLED::loop() {
  mLED.loop();
}

void StatusLED::on() {
  mLED.on();
}

void StatusLED::off() {
  mLED.off();
}

void StatusLED::connecting() {
  mLED.blink(100, 10);
}

void StatusLED::listening() {
  mLED.blink(10, 10000);
}

void StatusLED::commandReceived() {
  mLED.flash(100);
}
