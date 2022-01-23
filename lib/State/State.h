#pragma once

#include <ESP8266WiFi.h>

class State
{
public:
  State();
  ~State();

  struct {
    int miCurrent;
    int miDefault;
    int miFadeSpeed;
  } mLightLevel;

  struct {
    const char* msSSID;
    bool mbConnected;
    int8_t miRSSI;
    IPAddress mIP;
    String msMAC;
    const char* msMDNSName;
    const char* msMDNSNetwork;
    bool mbMDNSStarted;
  } mNetwork;

  struct {
    int miPort;
  } mWeb;

};
