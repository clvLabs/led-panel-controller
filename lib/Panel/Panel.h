#pragma once

#include "Relay.h"
#include "Dimmer.h"

class Panel
{
public:
  Panel();
  ~Panel();

  void loop();

  void setLevel(unsigned short level);

private:
  Relay mRelay;
  Dimmer mDimmer;
  unsigned short miLevel;
};
