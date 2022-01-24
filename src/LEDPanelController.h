#pragma once

#include <Arduino.h>
#include "EEPROMConfig.h"
#include "StatusLED.h"
#include "Network.h"
#include "WebServer.h"
#include "Panel.h"
#include "State.h"
#include "MQTT.h"
#include "Demo.h"

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
  void onDemoStarted();

  void onChangeLevel(uint8_t level);
  void onChangeDefault(uint8_t level);
  void onChangeFadeSpeed(uint8_t speed);

  EEPROMConfig mEEPROMCfg;
  StatusLED mStatusLED;
  Network mNetwork;
  WebServer mWebServer;
  Panel mPanel;
  State mState;
  MQTT mMQTT;
  Demo mDemoManager;
};
