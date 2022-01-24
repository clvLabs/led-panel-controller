#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController()
{}

LEDPanelController::~LEDPanelController() {}

void LEDPanelController::start() {
  mEEPROMCfg.read();
  mState.mLightLevel.miDefault = mEEPROMCfg.data.defaultDimLevel;
  mState.mLightLevel.miCurrent = mEEPROMCfg.data.defaultDimLevel;
  mState.mLightLevel.miFadeSpeed = mEEPROMCfg.data.fadeSpeed;
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

  mWebServer.onChangeLevel = std::bind(&LEDPanelController::onChangeLevel, this, std::placeholders::_1);
  mWebServer.onChangeDefault = std::bind(&LEDPanelController::onChangeDefault, this, std::placeholders::_1);
  mWebServer.onChangeFadeSpeed = std::bind(&LEDPanelController::onChangeFadeSpeed, this, std::placeholders::_1);

  mMQTT.onChangeLevel = std::bind(&LEDPanelController::onChangeLevel, this, std::placeholders::_1);
  mMQTT.onChangeDefault = std::bind(&LEDPanelController::onChangeDefault, this, std::placeholders::_1);
  mMQTT.onChangeFadeSpeed = std::bind(&LEDPanelController::onChangeFadeSpeed, this, std::placeholders::_1);

  mDemoManager.onDemoStarted = std::bind(&LEDPanelController::onDemoStarted, this);
  mDemoManager.onChangeLevel = std::bind(&LEDPanelController::onChangeLevel, this, std::placeholders::_1);
  mDemoManager.start(&mState);
}

void LEDPanelController::loop() {
  mDemoManager.loop();
  mNetwork.loop();
  mStatusLED.loop();
  mPanel.loop();

  // MQTT and WebServer are disabled in Demo mode
  if (!mDemoManager.isActive()) {
    mMQTT.loop();
    mWebServer.loop();
  }

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

  // Demo manager handles statusLED by itself
  if (!mDemoManager.isActive())
    mStatusLED.commandReceived();
}

void LEDPanelController::onDemoStarted() {
  mStatusLED.demoActive();
}

void LEDPanelController::onChangeDefault(uint8_t level) {
  if (level == mState.mLightLevel.miDefault)
    return;

  mState.mLightLevel.miDefault = level;
  mEEPROMCfg.data.defaultDimLevel = level;
  mEEPROMCfg.write();
  mStatusLED.commandReceived();
}

void LEDPanelController::onChangeFadeSpeed(uint8_t speed) {
  if (speed == mState.mLightLevel.miFadeSpeed)
    return;

  mState.mLightLevel.miFadeSpeed = speed;
  mEEPROMCfg.write();
  mStatusLED.commandReceived();
}
