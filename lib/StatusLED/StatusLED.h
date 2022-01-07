#pragma once

#include "LED.h"

class StatusLED
{
public:
  StatusLED();
  ~StatusLED();

  void setup(uint8_t pin, bool inverse=false);
  void loop();

  void on();
  void off();
  void connecting();
  void listening();
  void commandReceived();

private:
  uint8_t miPin;
  bool mbInverseLogic;
  LED mLED;
};
