#pragma once

#include <ESP8266WiFi.h>

class Network
{
public:
  Network();
  ~Network();

  void start();
  void loop();

  const char* getStatusStr();
  const char* getStatusStr(wl_status_t status);

  IPAddress localIP();
  String macAddress();
  bool isConnected();
};
