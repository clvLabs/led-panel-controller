#pragma once

#include "Relay.h"
#include "Dimmer.h"
#include "State.h"

class Panel
{
public:
  Panel();
  ~Panel();

  void start(State* state);
  void loop();

  void setLevel(uint8_t level);

private:
  State* mState;
  Relay mRelay;
  Dimmer mDimmer;
  uint8_t miLevel;
};
