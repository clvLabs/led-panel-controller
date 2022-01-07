#pragma once

#include "LED.h"

class StatusLED
{
public:
  StatusLED();
  ~StatusLED();

  void setup(int pin, bool inverse=false);
  void loop();

  void on();
  void off();
  void connecting();
  void listening();

private:
  int miPin;
  bool mbInverseLogic;
  LED mLED;
};
