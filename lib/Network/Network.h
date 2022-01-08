#pragma once

#include <ESP8266WiFi.h>

class Network
{
public:
  std::function< void() > onConnect;
  std::function< void() > onDisconnect;

  Network();
  ~Network();

  void start();
  void loop();

  void printConnectionInfo();
  const char* getStatusStr();
  const char* getStatusStr(wl_status_t status);

  IPAddress localIP();
  String macAddress();
  bool isConnected();

private:
  bool mbConnecting;
  uint32_t miLastConnectionCheck;

  void waitForConnection();
  bool checkDisconnection();
  void startMDNS();
  void startOTA();
};
