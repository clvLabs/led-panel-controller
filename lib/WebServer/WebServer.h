#pragma once

#include <ESP8266WebServer.h>
#include "State.h"

class WebServer
{
public:
  std::function< void(uint8_t) > onChangeLevel;
  std::function< void(uint8_t) > onChangeDefault;

  WebServer();
  ~WebServer();

  void start(State* state);
  void loop();

  void sendRedirect();

private:
  State* mState;
  ESP8266WebServer mServer;

  void handleNotFound();
  void handleIndexCSS();

  void handleHome();
  void handleInfo();
  void handleReboot();
  void handlePresetLevel(uint8_t level);
  void handleLevel();
  void handleDefault();
};
