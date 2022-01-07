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

void Dimmer::setLevel(unsigned short level) {
  if (level < PWM_MIN_VALUE)
    level = 0;

  miLevel = level;

  analogWrite(miPin, miLevel);
}
