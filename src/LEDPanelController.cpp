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
  Serial.println("Ready");
  mStatusLED.listening();
}

void LEDPanelController::onNetworkDisconnect() {
  Serial.println("NETWORK CONNECTION LOST !!! ");
  mStatusLED.connecting();
}

void LEDPanelController::onWebServerChangeLevel(uint8_t level) {
  if (level == mState.mLightLevel.miCurrent)
    return;

  mState.mLightLevel.miCurrent = level;
  mPanel.setLevel(mState.mLightLevel.miCurrent);
  Serial.print("Level changed by web request: ");
  Serial.println(mState.mLightLevel.miCurrent);
  mStatusLED.commandReceived();
}

void LEDPanelController::onWebServerChangeDefault(uint8_t level) {
  if (level == mState.mLightLevel.miDefault)
    return;

  mState.mLightLevel.miDefault = level;
  mEEPROMCfg.data.defaultDimLevel = level;
  mEEPROMCfg.write();
  Serial.print("Default level changed by web request: ");
  Serial.println(mState.mLightLevel.miDefault);
  mStatusLED.commandReceived();
}
