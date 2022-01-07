#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include "Config.h"
#include "Network.h"


Network::Network()
{}

Network::~Network()
{}

void Network::start() {
  Serial.println(" Starting network");
  uint32_t startTime = millis();
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection
  wl_status_t status = WiFi.status();
  while (status != WL_CONNECTED) {
    Serial.print("  - Network status: ");
    Serial.println(getStatusStr(status));

    // Total waiting time: 500ms
    for (int i=0; i<3; i++) {
      // ledON();
      delay(50);
      // ledOFF();
      delay(50);
    }
    delay(200);

    status = WiFi.status();
  }
  uint32_t endTime = millis();

  Serial.print("- Connected to ");
  Serial.print(WIFI_SSID);
  Serial.print(" - IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("- Connection time (ms): ");
  Serial.println(endTime-startTime);

  if (MDNS.begin(MDNS_NAME)) {
    Serial.println("- MDNS responder started as " MDNS_NAME "." MDNS_NETWORK);
  }
}

void Network::loop() {
  MDNS.update();
}


const char* Network::getStatusStr() {
  return getStatusStr(WiFi.status());
}

const char* Network::getStatusStr(wl_status_t status) {
  switch(status) {
    case WL_NO_SHIELD:        return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:      return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:    return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:   return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:        return "WL_CONNECTED";
    case WL_CONNECT_FAILED:   return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:  return "WL_CONNECTION_LOST";
    case WL_WRONG_PASSWORD:   return "WL_WRONG_PASSWORD";
    case WL_DISCONNECTED:     return "WL_DISCONNECTED";
    default:                  return "UNKNOWN";
  }
}

IPAddress Network::localIP() {
  return WiFi.localIP();
}

String Network::macAddress() {
  return WiFi.macAddress();
}

bool Network::isConnected() {
  return WiFi.isConnected();
}
