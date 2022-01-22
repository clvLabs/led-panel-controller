#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController() {}

LEDPanelController::~LEDPanelController() {}

void LEDPanelController::start() {
  mEEPROMCfg.read();
  mState.mLightLevel.miDefault = mEEPROMCfg.data.defaultDimLevel;
  mState.mLightLevel.miCurrent = mEEPROMCfg.data.defaultDimLevel;
  mPanel.start(&mState);

  mStatusLED.start();
  mStatusLED.connecting();

  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n\n");
  Serial.println(" led-panel-controller - LED panel #" PANEL_ID);
  Serial.println("-------------------------------------");

  mNetwork.onConnect = std::bind(&LEDPanelController::onNetworkConnect, this);
  mNetwork.onDisconnect = std::bind(&LEDPanelController::onNetworkDisconnect, this);
  mNetwork.start(&mState);

  mWebServer.onChangeLevel = std::bind(&LEDPanelController::onWebServerChangeLevel, this, std::placeholders::_1);
  mWebServer.onChangeDefault = std::bind(&LEDPanelController::onWebServerChangeDefault, this, std::placeholders::_1);

  mMQTT.onChangeLevel = std::bind(&LEDPanelController::onMQTTChangeLevel, this, std::placeholders::_1);
  mMQTT.onChangeDefault = std::bind(&LEDPanelController::onMQTTChangeDefault, this, std::placeholders::_1);
}

void LEDPanelController::loop() {
  mNetwork.loop();
  mWebServer.loop();
  mStatusLED.loop();
  mPanel.loop();
  mMQTT.loop();
  delay(1);
}

void LEDPanelController::onNetworkConnect() {
  mWebServer.start(&mState);
  mMQTT.start(&mState);
  Serial.println("[Main] Ready");
  mStatusLED.listening();
}

void LEDPanelController::onNetworkDisconnect() {
  mStatusLED.connecting();
}

void LEDPanelController::onChangeLevel(uint8_t level) {
  if (level == mState.mLightLevel.miCurrent)
    return;

  mState.mLightLevel.miCurrent = level;
  mPanel.setLevel(mState.mLightLevel.miCurrent);
  mStatusLED.commandReceived();
}

void LEDPanelController::onChangeDefault(uint8_t level) {
  if (level == mState.mLightLevel.miDefault)
    return;

  mState.mLightLevel.miDefault = level;
  mEEPROMCfg.data.defaultDimLevel = level;
  mEEPROMCfg.write();
  mStatusLED.commandReceived();
}

void LEDPanelController::onWebServerChangeLevel(uint8_t level) {
  onChangeLevel(level);
}

void LEDPanelController::onWebServerChangeDefault(uint8_t level) {
  onChangeDefault(level);
}

void LEDPanelController::onMQTTChangeLevel(uint8_t level) {
  onChangeLevel(level);
}

void LEDPanelController::onMQTTChangeDefault(uint8_t level) {
  onChangeDefault(level);
}
