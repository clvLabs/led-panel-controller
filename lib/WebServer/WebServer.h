#pragma once

#include <ESP8266WebServer.h>

class WebServer
{
public:
  std::function< void(unsigned short) > onChangeLevel;

  WebServer();
  ~WebServer();

  void start();
  void loop();

  void sendRedirect();

private:
  void handleNotFound();
  void handleIndexCSS();

  void handleHome();
  void handlePresetLevel(unsigned short level);
  void handleLevel();
  void handleDefault();

  ESP8266WebServer mServer;
};
