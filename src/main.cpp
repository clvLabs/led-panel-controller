#include <Arduino.h>
#include "LEDPanelController.h"

LEDPanelController controller;

void setup() {
  controller.start();
}

void loop() {
  controller.loop();
}
