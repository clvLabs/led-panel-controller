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

void Panel::setLevel(uint8_t level) {
  if (level > 100)
    level = 100;

  miLevel = level;

  if (level == 0) {
    mRelay.off();
  } else {
    mRelay.on();
  }

  mDimmer.setLevel(level);
}
