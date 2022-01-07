#include <Arduino.h>
#include "StatusLED.h"

StatusLED::StatusLED()
: miPin(0)
, mbInverseLogic(false)
{}

StatusLED::~StatusLED() {}

void StatusLED::setup(int pin, bool inverseLogic) {
  miPin = pin;
  mbInverseLogic = inverseLogic;
  mLED.setup(pin, inverseLogic);
}

void StatusLED::loop() {
  mLED.loop();
}
