#include <Arduino.h>
#include "MQTT.h"
#include "Config.h"

MQTT::MQTT()
: onChangeLevel(nullptr)
, onChangeDefault(nullptr)
, mMQTTClient(PubSubClient(mWiFiClient))
, mbStarted(false)
, mbConnected(false)
, miLastConnectionAttempt(0)
, miLastPublish(0)
{
}

MQTT::~MQTT() {}

void MQTT::start(State* state) {
  mState = state;

  Serial.println("[MQTT] Starting client");

  std::function<void(char*, uint8_t*, unsigned int)> callback = [=](char* topic, uint8_t* payload, unsigned int length) {
    this->onMQTTMessage(topic, payload, length);
  };

  mMQTTClient.setCallback(callback);
  mMQTTClient.setServer(MQTT_SERVER, MQTT_PORT);
  mMQTTClient.setKeepAlive(100);

  mbStarted = true;
  connect();
}

void MQTT::loop() {
  if (!mbStarted)
    return;

  if (!mMQTTClient.connected()) {
    if (mbConnected) {
      Serial.println("[MQTT] Connection LOST !!");
      mbConnected = false;
    }

    uint32_t elapsed = millis() - miLastConnectionAttempt;

    if (!miLastConnectionAttempt || elapsed >= MQTT_RECONNECT_DELAY)
      connect();
  } else {
    mbConnected = true;

    uint32_t elapsed = millis() - miLastPublish;

    if (elapsed >= 1000) {
      miLastPublish = millis();
      mMQTTClient.publish(MDNS_NAME "/" MQTT_STATUS_TOPIC, getStatusMsg().c_str());
    }

  }

  mMQTTClient.loop();
}

void MQTT::connect() {
  Serial.print("[MQTT] Attempting connection as ");
  Serial.println(MDNS_NAME);
  miLastConnectionAttempt = millis();

  if (mMQTTClient.connect(MDNS_NAME)) {
    mbConnected = true;
    Serial.println("[MQTT] Connected");

    mMQTTClient.subscribe(MDNS_NAME "/do/#");
    mMQTTClient.subscribe(MDNS_NAME "/set/#");
  } else {
    mbConnected = false;
    Serial.print("[MQTT] connection failed! state: ");
    Serial.println(mMQTTClient.state());
  }
}

void MQTT::onMQTTMessage(char* topic, uint8_t* payload, unsigned int length) {
  uint8_t pos = strlen(MDNS_NAME);
  char *subtopic = topic + pos;

  if (!strncmp(subtopic, "/do/on", 6))
    onMQTT_do_on(payload, length);
  else if (!strncmp(subtopic, "/do/off", 7))
    onMQTT_do_off(payload, length);
  else if (!strncmp(subtopic, "/do/lo", 6))
    onMQTT_do_lo(payload, length);
  else if (!strncmp(subtopic, "/do/med", 7))
    onMQTT_do_med(payload, length);
  else if (!strncmp(subtopic, "/do/hi", 6))
    onMQTT_do_hi(payload, length);
  else if (!strncmp(subtopic, "/do/up", 6))
    onMQTT_do_up(payload, length);
  else if (!strncmp(subtopic, "/do/down", 8))
    onMQTT_do_down(payload, length);
  else if (!strncmp(subtopic, "/do/reboot", 10))
    onMQTT_do_reboot(payload, length);
  else if (!strncmp(subtopic, "/set/level", 10))
    onMQTT_set_level(payload, length);
  else if (!strncmp(subtopic, "/set/default", 12))
    onMQTT_set_default(payload, length);
  else {
    Serial.print("[MQTT] UNKNOWN Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }
}

void MQTT::onMQTT_do_on(uint8_t* payload, unsigned int length) {
  Serial.println("[MQTT] Received do/on - new level: 100");
  if (onChangeLevel)
    onChangeLevel(100);
}

void MQTT::onMQTT_do_off(uint8_t* payload, unsigned int length) {
  Serial.println("[MQTT] Received do/off - new level: 0");
  if (onChangeLevel)
    onChangeLevel(0);
}

void MQTT::onMQTT_do_lo(uint8_t* payload, unsigned int length) {
  Serial.println("[MQTT] Received do/lo - new level: 1");
  if (onChangeLevel)
    onChangeLevel(1);
}

void MQTT::onMQTT_do_med(uint8_t* payload, unsigned int length) {
  Serial.println("[MQTT] Received do/med - new level: 50");
  if (onChangeLevel)
    onChangeLevel(50);
}

void MQTT::onMQTT_do_hi(uint8_t* payload, unsigned int length) {
  Serial.println("[MQTT] Received do/hi - new level: 100");
  if (onChangeLevel)
    onChangeLevel(100);
}

void MQTT::onMQTT_do_up(uint8_t* payload, unsigned int length) {
  uint8_t level;
  if (mState->mLightLevel.miCurrent >= 90)
    level = 100;
  else
    level = mState->mLightLevel.miCurrent + 10;

  Serial.print("[MQTT] Received do/up - new level: ");
  Serial.println(level);

  if (onChangeLevel)
    onChangeLevel(level);
}

void MQTT::onMQTT_do_down(uint8_t* payload, unsigned int length) {
  uint8_t level;
  if (mState->mLightLevel.miCurrent <= 10)
    level = 0;
  else
    level = mState->mLightLevel.miCurrent - 10;

  Serial.print("[MQTT] Received do/down - new level: ");
  Serial.println(level);

  if (onChangeLevel)
    onChangeLevel(level);
}

void MQTT::onMQTT_do_reboot(uint8_t* payload, unsigned int length) {
  Serial.println("[MQTT] Received do/reboot - WARNING: Rebooting as requested.");
  ESP.reset();
}

void MQTT::onMQTT_set_level(uint8_t* payload, unsigned int length) {
  if (length > 3) { // Ignore "strange" requests
    Serial.println("[MQTT] IGNORING set/level - payload TOO LARGE");
    return;
  }
  char buffer[5];
  memcpy((void*)buffer, (void*)payload, length);
  buffer[length] = 0;

  Serial.print("[MQTT] Received set/level - payload: ");
  Serial.println(buffer);
  uint8_t level = (uint8_t)atoi((char*)payload);
  if (onChangeLevel)
    onChangeLevel(level);
}

void MQTT::onMQTT_set_default(uint8_t* payload, unsigned int length) {
  if (length > 3) { // Ignore "strange" requests
    Serial.println("[MQTT] IGNORING set/default - payload TOO LARGE");
    return;
  }
  char buffer[5];
  memcpy((void*)buffer, (void*)payload, length);
  buffer[length] = 0;

  Serial.print("[MQTT] Received set/default - payload: ");
  Serial.println(buffer);
  uint8_t level = (uint8_t)atoi(buffer);

  if (onChangeDefault)
    onChangeDefault(level);
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
