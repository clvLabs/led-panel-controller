#include <Arduino.h>
#include "Dimmer.h"
#include "Config.h"

//
// The dimmer in the LIFUD controller expects a PWM value to regulate the light intensity:
// - Cable disconnected        - Max light level
// - PWM: 0                    - Max light level
// - PWM: PWM_ON_THRESHOLD     - Min light level
// - PWM: Anything until 255   - Light OFF
//
// This seems counterintuitive, but it helps keep the panel ON if no PWM cable is connected.
//
// The initial value for PWM_ON_THRESHOLD in Config.sample.h is based on
//  the calibration I did with my drivers, if you want to make sure you're getting
//  the whole range for your dimmer, do some tweaking with your PWM_ON_THRESHOLD.
//

Dimmer::Dimmer()
: miPin(PWM_DIM_PIN)
, miLevel(0)
{}

Dimmer::~Dimmer() {}

void Dimmer::start() {
  pinMode(miPin, OUTPUT);
}

void Dimmer::loop() {}

void Dimmer::setLevel(uint8_t level) {
  if (level > 100)
    level = 100;

  miLevel = level;

  uint8_t pwmLevel = 0;

  if (miLevel == 0) {
    digitalWrite(miPin, HIGH);
  } else if (miLevel == 100) {
    digitalWrite(miPin, LOW);
  } else {
    // We have 99 dim levels between PWM_ON_THRESHOLD and 0
    pwmLevel = PWM_ON_THRESHOLD - ( PWM_ON_THRESHOLD * (miLevel-1) / 99 );
    analogWrite(miPin, pwmLevel);
  }
}
