#include <Arduino.h>
#include "Demo.h"
#include "Config.h"

Demo::Demo()
: onDemoStarted(nullptr)
, onChangeLevel(nullptr)
, miPin(DEMO_JUMPER_PIN)
, mbActive(false)
, miNextDemoCheck(0)
, miNextDemoRSSI(0)
, miDemoPhase(0)
, miDemoIteration(0)
, mbDemoRampingUp(true)
{}

Demo::~Demo() {}

void Demo::start(State* state) {
  mState = state;

  pinMode(miPin, INPUT);
}

void Demo::loop() {
  if (!mbActive && millis() >= miNextDemoCheck) {

    if (digitalRead(miPin)) {
      Serial.println("[Demo] Starting DEMO mode, reset the board to stop");
      mbActive = true;
      if (onDemoStarted)
        onDemoStarted();
    } else {
      miNextDemoCheck = millis() + 5000;
    }
  }

  if (!mbActive)
    return;

  if (millis() >= miNextDemoRSSI) {
    miNextDemoRSSI = millis() + 1000;
    Serial.print("[Demo] Current WiFi RSSI: ");
    Serial.println(mState->mNetwork.miRSSI);
  }

  if (millis() < miNextDemoCheck)
    return;

  uint8_t step = 1;
  uint8_t min = 1;
  uint8_t max = 99;
  uint8_t iterations = 1;
  unsigned long sleep = 50;
  const uint8_t NUM_PHASES = 3;

  switch(miDemoPhase) {
    case 0:
      iterations = 3;
      step = 5;
      max = 50;
    break;

    case 1:
    break;

    case 2:
      iterations = 10;
      max = 10;
      sleep = 120;
    break;

    default:
    break;
  }

  uint8_t currLevel = mState->mLightLevel.miCurrent;
  uint8_t newLevel = currLevel;

  if (mbDemoRampingUp) {
    if (currLevel == max) {
      mbDemoRampingUp = !mbDemoRampingUp;
    } else if (currLevel + step > max) {
      newLevel = max;
    } else {
      newLevel = currLevel + step;
    }
  } else {
    if (currLevel == min) {
      mbDemoRampingUp = !mbDemoRampingUp;
      miDemoIteration++;
      if (miDemoIteration >= iterations) {
        miDemoPhase++;
        miDemoPhase %= NUM_PHASES;
        miDemoIteration = 0;
      }
    } else if (min + step > currLevel) {
      newLevel = min;
    } else {
      newLevel = currLevel - step;
    }
  }

  if (onChangeLevel)
    onChangeLevel(newLevel);

  miNextDemoCheck = millis() + sleep;
}

bool Demo::isActive() {
  return mbActive;
}
