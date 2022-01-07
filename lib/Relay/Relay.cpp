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

    if (on) {
      digitalWrite(miSetPin, HIGH);
      digitalWrite(miRstPin, LOW);
    } else {
      digitalWrite(miSetPin, LOW);
      digitalWrite(miRstPin, HIGH);
    }
  }
  mbOn = on;
}
