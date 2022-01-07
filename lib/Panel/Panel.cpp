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
  if (level > 100)
    level = 100;

  miLevel = level;

  if (level == 0) {
    mRelay.setState(false);
  } else {
    mRelay.setState(true);
  }

  mDimmer.setLevel(level);
}
