#pragma once
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "State.h"

class MQTT
{
public:
  MQTT();
  ~MQTT();

  void start(State* state);
  void loop();

private:
  State* mState;
  WiFiClient mWiFiClient;
  PubSubClient mMQTTClient;
  bool mbStarted;
  bool mbConnected;
  uint32_t miLastConnectionAttempt;
  uint32_t miLastPublish;

  void connect();
  void onMQTTMessage(char* topic, uint8_t* payload, unsigned int length);
  String getStatusMsg();
};
