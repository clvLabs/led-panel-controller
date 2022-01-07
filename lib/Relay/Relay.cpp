#include <Arduino.h>
#include "Relay.h"
#include "Config.h"

Relay::Relay()
: miSetPin(RELAY_SET_PIN)
, miRstPin(RELAY_RST_PIN)
, mbOn(false)
{
  pinMode(miSetPin, OUTPUT);
  pinMode(miRstPin, OUTPUT);
}

Relay::~Relay() {}

void Relay::loop() {}

void Relay::setState(bool on) {
  if (on != mbOn) {
    // Manage change
  }
  mbOn = on;
}
