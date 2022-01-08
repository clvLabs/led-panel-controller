#include <Arduino.h>
#include "Relay.h"
#include "Config.h"

Relay::Relay()
: miSetPin(RELAY_SET_PIN)
, miRstPin(RELAY_RST_PIN)
, mbOn(false)
, mbCoilActive(false)
, miCoilActivationTime(0)
{}

Relay::~Relay() {}

void Relay::start() {
  pinMode(miSetPin, OUTPUT);
  pinMode(miRstPin, OUTPUT);

  _off();
}

void Relay::loop() {
  _checkCoil();
}

void Relay::on() {
  if (!mbOn)
    _on();
}

void Relay::off() {
  if (mbOn)
    _off();
}

void Relay::_checkCoil() {
  if (mbCoilActive) {
    uint32_t elapsed = millis() - miCoilActivationTime;
    if (elapsed >= RELAY_COIL_ACTIVATION_TIME) {
      digitalWrite(miSetPin, LOW);
      digitalWrite(miRstPin, LOW);
      mbCoilActive = false;
    }
  }
}

void Relay::_on() {
  digitalWrite(miSetPin, HIGH);
  digitalWrite(miRstPin, LOW);
  mbOn = true;
  mbCoilActive = true;
  miCoilActivationTime = millis();
}

void Relay::_off() {
  digitalWrite(miSetPin, LOW);
  digitalWrite(miRstPin, HIGH);
  mbOn = false;
  mbCoilActive = true;
  miCoilActivationTime = millis();
}
