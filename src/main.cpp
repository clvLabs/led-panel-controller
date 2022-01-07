#include <Arduino.h>
#include "LEDPanelController.h"

LEDPanelController controller;

void setup() {
  controller.setup();
}

void loop() {
  controller.loop();
}
