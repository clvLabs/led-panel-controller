#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController()
: miLevel(0)
{}

LEDPanelController::~LEDPanelController()
{}

void LEDPanelController::onNetworkConnect() {
  Serial.println("Listening");
  statusLED.listening();
}

void LEDPanelController::onNetworkDisconnect() {
  Serial.println("NETWORK CONNECTION LOST !!! ");
  statusLED.connecting();
}

void LEDPanelController::onWebServerChangeLevel(uint8_t level) {
  miLevel = level;
  panel.setLevel(miLevel);
}

void LEDPanelController::onWebServerChangeDefault(uint8_t level) {
  eepromCfg.data.level = level;
  eepromCfg.write();
}

void LEDPanelController::setup() {
  eepromCfg.read();
  miLevel = eepromCfg.data.level;
  panel.setLevel(miLevel);

  statusLED.setup(STATUSLED_PIN, STATUSLED_INVERTED);
  statusLED.connecting();

  Serial.begin(115200);
  delay(100);
  Serial.println("\n\n\n");
  Serial.println(" led-panel-controller - LED panel #" PANEL_ID);
  Serial.println("-------------------------------------");

  network.onConnect = std::bind(&LEDPanelController::onNetworkConnect, this);
  network.onDisconnect = std::bind(&LEDPanelController::onNetworkDisconnect, this);
  network.start();

  webServer.onChangeLevel = std::bind(&LEDPanelController::onWebServerChangeLevel, this, std::placeholders::_1);
  webServer.onChangeDefault = std::bind(&LEDPanelController::onWebServerChangeDefault, this, std::placeholders::_1);
  webServer.start();
}

void LEDPanelController::loop() {
  network.loop();
  webServer.loop();
  statusLED.loop();
  panel.loop();
}
