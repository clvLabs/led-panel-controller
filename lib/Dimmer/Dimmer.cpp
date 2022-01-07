#include <Arduino.h>
#include "Dimmer.h"
#include "Config.h"

Dimmer::Dimmer()
: miPin(PWM_DIM_PIN)
, miLevel(0)
{
  pinMode(miPin, OUTPUT);
}

Dimmer::~Dimmer() {}

void Dimmer::loop() {}

void Dimmer::setLevel(uint8_t level) {
  if (level > 100)
    level = 100;

  miLevel = level;

  uint8_t pwmLevel = 0;

  if (miLevel)
    pwmLevel = PWM_MIN_VALUE + ((PWM_MAX_VALUE - PWM_MIN_VALUE) * miLevel / 100);

  analogWrite(miPin, pwmLevel);
}
