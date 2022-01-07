#include <Arduino.h>
#include "StatusLED.h"

StatusLED::StatusLED()
: miPin(0)
, mbInverseLogic(false)
{}

StatusLED::~StatusLED() {}

void StatusLED::setup(uint8_t pin, bool inverseLogic) {
  miPin = pin;
  mbInverseLogic = inverseLogic;
  mLED.setup(pin, inverseLogic);
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
  mLED.blink(100, 100);
}

void StatusLED::listening() {
  mLED.blink(10, 10000);
}
