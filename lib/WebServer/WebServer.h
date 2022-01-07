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

  void sendResultPage();
  void sendRedirect();

private:
  void handleRoot();
  void handleFast();
  void handleNormal();
  void handleSlow();
  void handleIndexCSS();
  void handleNotFound();

  ESP8266WebServer mServer;
};
