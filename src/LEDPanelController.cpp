#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController() {}

LEDPanelController::~LEDPanelController() {}

void LEDPanelController::start() {
  eepromCfg.read();
  mState.mLightLevel.miDefault = eepromCfg.data.level;
  mState.mLightLevel.miCurrent = eepromCfg.data.level;
  panel.start(&mState);

  statusLED.start();
  statusLED.connecting();

  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n\n");
  Serial.println(" led-panel-controller - LED panel #" PANEL_ID);
  Serial.println("-------------------------------------");

  network.onConnect = std::bind(&LEDPanelController::onNetworkConnect, this);
  network.onDisconnect = std::bind(&LEDPanelController::onNetworkDisconnect, this);
  network.start(&mState);

  webServer.onChangeLevel = std::bind(&LEDPanelController::onWebServerChangeLevel, this, std::placeholders::_1);
  webServer.onChangeDefault = std::bind(&LEDPanelController::onWebServerChangeDefault, this, std::placeholders::_1);
  webServer.start(&mState);
}

void LEDPanelController::loop() {
  network.loop();
  webServer.loop();
  statusLED.loop();
  panel.loop();
  delay(10);
}

void LEDPanelController::onNetworkConnect() {
  Serial.println("Listening");
  statusLED.listening();
}

void LEDPanelController::onNetworkDisconnect() {
  Serial.println("NETWORK CONNECTION LOST !!! ");
  statusLED.connecting();
}

void LEDPanelController::onWebServerChangeLevel(uint8_t level) {
  if (level == mState.mLightLevel.miCurrent)
    return;

  mState.mLightLevel.miCurrent = level;
  panel.setLevel(mState.mLightLevel.miCurrent);
  Serial.print("Level changed by web request: ");
  Serial.println(mState.mLightLevel.miCurrent);
  statusLED.commandReceived();
}

void LEDPanelController::onWebServerChangeDefault(uint8_t level) {
  if (level == mState.mLightLevel.miDefault)
    return;

  mState.mLightLevel.miDefault = level;
  eepromCfg.data.level = level;
  eepromCfg.write();
  Serial.print("Default level changed by web request: ");
  Serial.println(mState.mLightLevel.miDefault);
  statusLED.commandReceived();
}
