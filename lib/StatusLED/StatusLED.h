#pragma once

#include "LED.h"

class StatusLED
{
public:
  StatusLED();
  ~StatusLED();

  void start();
  void loop();

  void on();
  void off();
  void connecting();
  void listening();
  void demoActive();
  void commandReceived();

private:
  LED mLED;
};
