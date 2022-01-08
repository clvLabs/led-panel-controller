#pragma once

#include "Relay.h"
#include "Dimmer.h"

class Panel
{
public:
  Panel();
  ~Panel();

  void start();
  void loop();

  void setLevel(uint8_t level);

private:
  Relay mRelay;
  Dimmer mDimmer;
  uint8_t miLevel;
};
