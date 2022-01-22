#pragma once

#include <Arduino.h>
#include "EEPROMConfig.h"
#include "StatusLED.h"
#include "Network.h"
#include "WebServer.h"
#include "Panel.h"
#include "State.h"
#include "MQTT.h"

class LEDPanelController
{
public:
  LEDPanelController();
  ~LEDPanelController();

  void start();
  void loop();

private:
  void onNetworkConnect();
  void onNetworkDisconnect();

  void onWebServerChangeLevel(uint8_t level);
  void onWebServerChangeDefault(uint8_t level);

  EEPROMConfig mEEPROMCfg;
  StatusLED mStatusLED;
  Network mNetwork;
  WebServer mWebServer;
  Panel mPanel;
  State mState;
  MQTT mMQTT;
};
