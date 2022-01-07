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
  void onWebServerChangeLevel(uint8_t level);

  StatusLED statusLED;
  Network network;
  WebServer webServer;
  Panel panel;

  uint8_t miLevel;
};
