#pragma once

#include <ESP8266WebServer.h>

class WebServer
{
public:
  std::function< void(uint8_t) > onChangeLevel;
  std::function< void(uint8_t) > onChangeDefault;

  WebServer();
  ~WebServer();

  void start();
  void loop();

  void sendRedirect();

private:
  void handleNotFound();
  void handleIndexCSS();

  void handleHome();
  void handlePresetLevel(uint8_t level);
  void handleLevel();
  void handleDefault();

  ESP8266WebServer mServer;
};
