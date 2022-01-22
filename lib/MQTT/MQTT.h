#pragma once
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "State.h"

class MQTT
{
public:
  std::function< void(uint8_t) > onChangeLevel;
  std::function< void(uint8_t) > onChangeDefault;

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
  void onMQTT_do_on(uint8_t* payload, unsigned int length);
  void onMQTT_do_off(uint8_t* payload, unsigned int length);
  void onMQTT_do_lo(uint8_t* payload, unsigned int length);
  void onMQTT_do_med(uint8_t* payload, unsigned int length);
  void onMQTT_do_hi(uint8_t* payload, unsigned int length);
  void onMQTT_do_up(uint8_t* payload, unsigned int length);
  void onMQTT_do_down(uint8_t* payload, unsigned int length);
  void onMQTT_do_reboot(uint8_t* payload, unsigned int length);
  void onMQTT_set_level(uint8_t* payload, unsigned int length);
  void onMQTT_set_default(uint8_t* payload, unsigned int length);
  String getStatusMsg();
};
