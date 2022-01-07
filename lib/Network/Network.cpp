#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include "Config.h"
#include "Network.h"


Network::Network()
: mbConnecting(false)
, miLastConnectionCheck(0)
{}

Network::~Network()
{}

void Network::start() {
  Serial.println("Starting network");
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  mbConnecting = true;
}

void Network::waitForConnection() {
  if (isConnected()) {
    mbConnecting = false;
    miLastConnectionCheck = millis();

    Serial.print("Connected to ");
    Serial.print(WIFI_SSID);
    Serial.print(" - IP address: ");
    Serial.println(localIP());

    if (MDNS.begin(MDNS_NAME)) {
      Serial.println("MDNS responder started as " MDNS_NAME "." MDNS_NETWORK);
    } else {
      Serial.println("COULDN'T start MDNS responder as " MDNS_NAME "." MDNS_NETWORK);
    }

    if (onConnect)
      onConnect();
  }
}

bool Network::checkDisconnection() {
  // Periodically check for disconnection
  uint32_t elapsed = millis() - miLastConnectionCheck;

  if (elapsed >= NETWORK_CONNECTION_CHECK_DELAY) {
    miLastConnectionCheck = millis();

    if (!isConnected()) {
      mbConnecting = true;

      if (onDisconnect)
        onDisconnect();

      return true;
    }
  }

  return false;
}

void Network::loop() {
  if (mbConnecting) {
    waitForConnection();
    return;
  }

  if (!checkDisconnection()) {
    MDNS.update();
  }
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
