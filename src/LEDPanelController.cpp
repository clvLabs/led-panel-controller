#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController()
: miLevel(0)
{}

LEDPanelController::~LEDPanelController()
{}

void LEDPanelController::onWebServerChangeLevel(uint8_t level) {
  miLevel = level;
  panel.setLevel(miLevel);
}

void LEDPanelController::setup() {
  miLevel = PANEL_DEFAULT_LEVEL;
  panel.setLevel(miLevel);

  statusLED.setup(STATUSLED_PIN, STATUSLED_INVERTED);
  statusLED.connecting();

  Serial.begin(9600);
  delay(100);
  Serial.println("\n\n\n");
  Serial.println(" led-panel-controller - LED panel #" PANEL_ID);
  Serial.println("-------------------------------------");

  network.start();

  webServer.onChangeLevel = std::bind(&LEDPanelController::onWebServerChangeLevel, this, std::placeholders::_1);
  webServer.start();

  Serial.println("- Listening");
  statusLED.listening();
}

void LEDPanelController::loop() {
  network.loop();
  webServer.loop();
  statusLED.loop();
  panel.loop();
}
