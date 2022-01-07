#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController()
: miLevel(0)
{}

LEDPanelController::~LEDPanelController()
{}

void LEDPanelController::onWebServerChangeLevel(unsigned short level) {
  miLevel = level;
  panel.setLevel(miLevel);
}

void LEDPanelController::setup() {
  statusLED.setup(STATUSLED_PIN, STATUSLED_INVERTED);

  Serial.begin(9600);
  delay(100);
  Serial.println("\n\n\n");
  Serial.println(" led-panel-controller - LED panel #" LEDPANEL_ID);
  Serial.println("-------------------------------------");

  network.start();

  webServer.onChangeLevel = std::bind(&LEDPanelController::onWebServerChangeLevel, this, std::placeholders::_1);
  webServer.start();

  onWebServerChangeLevel(80);

  Serial.println("- Listening");
}

void LEDPanelController::loop() {
  network.loop();
  webServer.loop();
  statusLED.loop();
  panel.loop();
}
