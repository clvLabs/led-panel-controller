#pragma once

#include <Arduino.h>
#include "StatusLED.h"
#include "Network.h"
#include "WebServer.h"
#include "Panel.h"

class LEDPanelController
{
public:
  LEDPanelController();
  ~LEDPanelController();

  void setup();
  void loop();

private:
  void onWebServerChangeLevel(unsigned short level);

  StatusLED statusLED;
  Network network;
  WebServer webServer;
  Panel panel;

  unsigned short miLevel;
};
