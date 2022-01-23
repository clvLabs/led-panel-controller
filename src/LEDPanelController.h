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
  void standardLoop();
  void demoLoop();

  void onNetworkConnect();
  void onNetworkDisconnect();

  void onChangeLevel(uint8_t level);
  void onChangeDefault(uint8_t level);
  void onChangeFadeSpeed(uint8_t speed);

  void onWebServerChangeLevel(uint8_t level);
  void onWebServerChangeDefault(uint8_t level);
  void onWebServerChangeFadeSpeed(uint8_t speed);

  void onMQTTChangeLevel(uint8_t level);
  void onMQTTChangeDefault(uint8_t level);
  void onMQTTChangeFadeSpeed(uint8_t speed);

  EEPROMConfig mEEPROMCfg;
  StatusLED mStatusLED;
  Network mNetwork;
  WebServer mWebServer;
  Panel mPanel;
  State mState;
  MQTT mMQTT;

  unsigned long miNextDemoCheck;
  unsigned long miNextDemoRSSI;
  bool mbDemoActive;
  uint8_t miDemoPhase;
  uint8_t miDemoIteration;
  bool mbDemoRampingUp;
};
