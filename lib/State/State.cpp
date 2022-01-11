#include <Arduino.h>
#include "State.h"
#include "Config.h"

State::State()
{
  mLightLevel.miCurrent = 0;
  mLightLevel.miDefault = 0;

  mNetwork.msSSID = WIFI_SSID;
  mNetwork.mbConnected = false;
  mNetwork.miRSSI = 0;
  mNetwork.mIP = IPAddress();
  mNetwork.msMAC = "";
  mNetwork.msMDNSName = MDNS_NAME;
  mNetwork.msMDNSNetwork = MDNS_NETWORK;
  mNetwork.mbMDNSStarted = false;

  mWeb.miPort = WEB_SERVER_PORT;
}

State::~State() {}
