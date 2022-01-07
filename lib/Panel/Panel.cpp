#include <Arduino.h>
#include "Panel.h"
#include "Config.h"

Panel::Panel()
: miLevel(0)
{}

Panel::~Panel() {}

void Panel::loop() {
  mRelay.loop();
  mDimmer.loop();
}

void Panel::setLevel(unsigned short level) {
  miLevel = level;

  if (level < PWM_MIN_VALUE) {
    mRelay.setState(false);
  } else {
    mRelay.setState(true);
  }

  mDimmer.setLevel(level);
}
