#include <Arduino.h>
#include "Relay.h"
#include "Config.h"

Relay::Relay()
: miSetPin(RELAY_SET_PIN)
, miRstPin(RELAY_RST_PIN)
, mbOn(false)
, mbCoilActive(false)
, miCoilActivationTime(0)
, miLastStateChange(0)
{}

Relay::~Relay() {}

void Relay::start() {
  pinMode(miSetPin, OUTPUT);
  pinMode(miRstPin, OUTPUT);

  _setState(false);
}

void Relay::loop() {
  _checkCoil();

  if (mbThrottledValueWaiting && miLastStateChange + PWM_FASTEST_CHANGE <= millis()) {
    mbThrottledValueWaiting = false;
    _setState(mbThrottledValue);
  }
}

void Relay::on() {
  if (!mbOn)
    _setState(true);
}

void Relay::off() {
  if (mbOn)
    _setState(false);
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

void Relay::_setState(bool state) {

  if (miLastStateChange + RELAY_FASTEST_CHANGE > millis()) {
    mbThrottledValueWaiting = true;
    mbThrottledValue = state;
    return;
  }

  digitalWrite(miSetPin, state ? HIGH : LOW);
  digitalWrite(miRstPin, state ? LOW : HIGH);
  mbOn = state;
  miLastStateChange = millis();

  mbCoilActive = true;
  miCoilActivationTime = millis();
}
