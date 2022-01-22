#include <Arduino.h>
#include "MQTT.h"
#include "Config.h"

MQTT::MQTT()
: mMQTTClient(PubSubClient(mWiFiClient))
, mbStarted(false)
, mbConnected(false)
, miLastConnectionAttempt(0)
, miLastPublish(0)
{
}

MQTT::~MQTT() {}

void MQTT::start(State* state) {
  mState = state;

  Serial.println("Starting MQTT client");

  std::function<void(char*, uint8_t*, unsigned int)> callback = [=](char* topic, uint8_t* payload, unsigned int length) {
    this->onMQTTMessage(topic, payload, length);
  };

  mMQTTClient.setCallback(callback);
  mMQTTClient.setServer(MQTT_SERVER, MQTT_PORT);

  mbStarted = true;
  connect();
}

void MQTT::loop() {
  if (!mbStarted)
    return;

  if (!mMQTTClient.connected()) {
    if (mbConnected) {
      Serial.println("MQTT connection LOST !!");
      mbConnected = false;
    }

    uint32_t elapsed = millis() - miLastConnectionAttempt;

    if (!miLastConnectionAttempt || elapsed >= MQTT_RECONNECT_DELAY)
      connect();
  } else {
    uint32_t elapsed = millis() - miLastPublish;

    if (elapsed >= 1000) {
      miLastPublish = millis();
      mMQTTClient.publish(MDNS_NAME "/" MQTT_STATUS_TOPIC, getStatusMsg().c_str());
    }

  }

  mMQTTClient.loop();
}

void MQTT::connect() {
  Serial.print("Attempting MQTT connection as ");
  Serial.println(MDNS_NAME);
  miLastConnectionAttempt = millis();

  if (mMQTTClient.connect(MDNS_NAME)) {
    mbConnected = true;
    Serial.println("MQTT connected");

    mMQTTClient.subscribe("inTopic");
  } else {
    mbConnected = false;
    Serial.print("MQTT connection failed! state: ");
    Serial.println(mMQTTClient.state());
  }
}

void MQTT::onMQTTMessage(char* topic, uint8_t* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

String MQTT::getStatusMsg() {
  String str = "";

  str += "{";
  str +=    "\"lightLevel\": {";
  str +=      "\"current\": ";
  str +=        mState->mLightLevel.miCurrent;
  str +=      ",";
  str +=      "\"default\": ";
  str +=        mState->mLightLevel.miDefault;
  str +=    "},";
  str +=    "\"network\": {";
  str +=      "\"rssi\": ";
  str +=        mState->mNetwork.miRSSI;
  str +=    "},";
  str +=    "\"misc\": {";
  str +=      "\"uptime\": ";
  str +=        millis() / 1000;
  str +=    "}";
  str += "}";

  return str;
}
