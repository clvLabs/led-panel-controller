#pragma once

#include "LED.h"

class StatusLED
{
public:
  StatusLED();
  ~StatusLED();

  void setup(int pin, bool inverse=false);
  void loop();

private:
  int miPin;
  bool mbInverseLogic;
  LED mLED;
};
