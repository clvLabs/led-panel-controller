#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
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
  WiFi.persistent(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  mbConnecting = true;
}

void Network::loop() {
  if (mbConnecting) {
    waitForConnection();
    return;
  }

  if (!checkDisconnection()) {
    MDNS.update();
    ArduinoOTA.handle();
  }
}

void Network::printConnectionInfo() {
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
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

void Network::waitForConnection() {
  if (isConnected()) {
    mbConnecting = false;
    miLastConnectionCheck = millis();

    printConnectionInfo();
    startMDNS();
    startOTA();

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

void Network::startMDNS() {
  Serial.println("Starting MDNS responder as: " MDNS_NAME "." MDNS_NETWORK);
  if (MDNS.begin(MDNS_NAME)) {
    Serial.println("MDNS responder started successfully");
  } else {
    Serial.println("COULDN'T start MDNS responder");
  }
}

void Network::startOTA() {
  ArduinoOTA.onStart([]() {
    Serial.println("[OTA] Start");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\n[OTA] End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)         Serial.println("[OTA] Auth Failed");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("[OTA] Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("[OTA] Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("[OTA] Receive Failed");
    else if (error == OTA_END_ERROR)     Serial.println("[OTA] End Failed");
  });

  Serial.println("Starting OTA server");
  ArduinoOTA.setPort(OTA_PORT);
  ArduinoOTA.setHostname(MDNS_NAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
}
