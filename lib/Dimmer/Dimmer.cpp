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
, miFadeLevel(0)
, miFadeSpeed(0)
, miLastPWMChange(0)
, mbThrottledValueWaiting(false)
, mbThrottledWaitingFade(false)
, miThrottledValue(0)
, miThrottledSpeed(0)
{}

Dimmer::~Dimmer() {}

void Dimmer::start() {
  pinMode(miPin, OUTPUT);
}

void Dimmer::loop() {
  if (mbThrottledValueWaiting && miLastPWMChange + PWM_FASTEST_CHANGE <= millis()) {
    mbThrottledValueWaiting = false;

    if (mbThrottledWaitingFade) {
      mbThrottledWaitingFade = false;
      fade(miThrottledValue, miThrottledSpeed);
    } else {
      setLevel(miThrottledValue);
    }
  }

  if (miFadeLevel != miLevel) {
    if (miLastPWMChange + PWM_FASTEST_CHANGE < millis()) {
      if (miFadeLevel < miLevel) {
        // Fading up
        if (miFadeLevel + miFadeSpeed > miLevel) {
          miFadeLevel = miLevel;
        } else {
          miFadeLevel += miFadeSpeed;
        }
      } else {
        // Fading down
        if (miLevel + miFadeSpeed > miFadeLevel) {
          miFadeLevel = miLevel;
        } else {
          miFadeLevel -= miFadeSpeed;
        }
      }
      _updatePWM(miFadeLevel);
    }
  }
}

void Dimmer::setLevel(uint8_t level) {
  if (level > 100)
    level = 100;

  if (level == miLevel)
    return;

  if (miLastPWMChange + PWM_FASTEST_CHANGE > millis()) {
    mbThrottledValueWaiting = true;
    mbThrottledWaitingFade = false;
    miThrottledValue = level;
    return;
  }

  miFadeLevel = level;
  miLevel = level;

  _updatePWM(level);
}

void Dimmer::fade(uint8_t level, uint8_t speed) {
  if (level > 100)
    level = 100;

  if (level == miLevel)
    return;

  if (miLastPWMChange + PWM_FASTEST_CHANGE > millis()) {
    mbThrottledValueWaiting = true;
    mbThrottledWaitingFade = true;
    miThrottledValue = level;
    miThrottledSpeed = speed;
    return;
  }

  miFadeLevel = miLevel;
  miLevel = level;
  miFadeSpeed = speed;
}

void Dimmer::_updatePWM(uint8_t level) {
  uint8_t pwmLevel = 0;

  if (level == 0) {
    digitalWrite(miPin, HIGH);
  } else if (level == 100) {
    digitalWrite(miPin, LOW);
  } else {
    // We have 99 dim levels between PWM_ON_THRESHOLD and 0
    pwmLevel = PWM_ON_THRESHOLD - ( PWM_ON_THRESHOLD * (level-1) / 99 );
    analogWrite(miPin, pwmLevel);
  }

  miLastPWMChange = millis();
}
