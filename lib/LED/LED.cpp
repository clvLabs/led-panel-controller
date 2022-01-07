#include <Arduino.h>
#include "LED.h"

LED::LED()
: miPin(0)
, mbInverseLogic(false)
, miLowValue(LOW)
, miHighValue(HIGH)
, mbLEDIsOn(false)
, mbBlinking(false)
, miOnTime(0)
, miOffTime(0)
, miLastLEDStateChange(0)
{}

LED::~LED() {}

void LED::setup(int pin, bool inverseLogic) {
  miPin = pin;
  mbInverseLogic = inverseLogic;
  pinMode(miPin, OUTPUT);

  if (mbInverseLogic) {
    miLowValue = HIGH;
    miHighValue = LOW;
  }

  off();
}

void LED::loop() {
  if (mbBlinking) {
      unsigned long elapsed = millis() - miLastLEDStateChange;

      if (mbLEDIsOn && elapsed > miOnTime) {
        _off();
      }
      else if (!mbLEDIsOn && elapsed > miOffTime) {
        _on();
      }
  }
}

void LED::on() {
  _on();
  mbBlinking = false;
}

void LED::off() {
  _off();
  mbBlinking = false;
}

void LED::blink(unsigned long onTime, unsigned long offTime) {
  miOnTime = onTime;
  miOffTime = offTime;
  mbBlinking = true;
  _on();
}

void LED::_on() {
  digitalWrite(miPin, miHighValue);
  miLastLEDStateChange = millis();
  mbLEDIsOn = true;
}

void LED::_off() {
  digitalWrite(miPin, miLowValue);
  miLastLEDStateChange = millis();
  mbLEDIsOn = false;
}
