#include <Arduino.h>
#include "Panel.h"
#include "Config.h"

Panel::Panel()
: miLevel(0)
{
  setLevel(miLevel);
}

Panel::~Panel() {}

void Panel::start(State* state) {
  mState = state;

  mRelay.start();
  mDimmer.start();

  setLevel(mState->mLightLevel.miCurrent);
}

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

  if (mState && mState->mLightLevel.miFadeSpeed > 0)
    mDimmer.fade(level, mState->mLightLevel.miFadeSpeed);
  else
    mDimmer.setLevel(level);
}
