#include "Config.h"
#include "LEDPanelController.h"


LEDPanelController::LEDPanelController()
: miNextDemoCheck(0)
, miNextDemoRSSI(0)
, mbDemoActive(false)
, miDemoPhase(0)
, miDemoIteration(0)
, mbDemoRampingUp(true)
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

  pinMode(DEMO_JUMPER_PIN, INPUT);

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
  mWebServer.onChangeFadeSpeed = std::bind(&LEDPanelController::onWebServerChangeFadeSpeed, this, std::placeholders::_1);

  mMQTT.onChangeLevel = std::bind(&LEDPanelController::onMQTTChangeLevel, this, std::placeholders::_1);
  mMQTT.onChangeDefault = std::bind(&LEDPanelController::onMQTTChangeDefault, this, std::placeholders::_1);
  mMQTT.onChangeFadeSpeed = std::bind(&LEDPanelController::onMQTTChangeFadeSpeed, this, std::placeholders::_1);
}

void LEDPanelController::loop() {
  if (!mbDemoActive && millis() >= miNextDemoCheck) {

    if (digitalRead(DEMO_JUMPER_PIN)) {
      Serial.println("[Main] Starting DEMO mode, reset the board to stop");
      mbDemoActive = true;
      mStatusLED.demoActive();
    } else {
      miNextDemoCheck = millis() + 5000;
    }
  }

  if (mbDemoActive)
    demoLoop();
  else
    standardLoop();
}

void LEDPanelController::standardLoop() {
  mNetwork.loop();
  mMQTT.loop();
  mWebServer.loop();
  mStatusLED.loop();
  mPanel.loop();
  delay(1);
}

void LEDPanelController::demoLoop() {
  mNetwork.loop();
  mStatusLED.loop();
  mPanel.loop();

  if (millis() >= miNextDemoRSSI) {
    miNextDemoRSSI = millis() + 1000;
    Serial.print("[DEMO] Current WiFi RSSI: ");
    Serial.println(mState.mNetwork.miRSSI);
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

  uint8_t currLevel = mState.mLightLevel.miCurrent;
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

  mPanel.setLevel(newLevel);
  mState.mLightLevel.miCurrent = newLevel;

  miNextDemoCheck = millis() + sleep;
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

void LEDPanelController::onChangeFadeSpeed(uint8_t speed) {
  if (speed == mState.mLightLevel.miFadeSpeed)
    return;

  mState.mLightLevel.miFadeSpeed = speed;
  mEEPROMCfg.write();
  mStatusLED.commandReceived();
}

void LEDPanelController::onWebServerChangeLevel(uint8_t level) {
  onChangeLevel(level);
}

void LEDPanelController::onWebServerChangeDefault(uint8_t level) {
  onChangeDefault(level);
}

void LEDPanelController::onWebServerChangeFadeSpeed(uint8_t level) {
  onChangeFadeSpeed(level);
}

void LEDPanelController::onMQTTChangeLevel(uint8_t level) {
  onChangeLevel(level);
}

void LEDPanelController::onMQTTChangeDefault(uint8_t level) {
  onChangeDefault(level);
}

void LEDPanelController::onMQTTChangeFadeSpeed(uint8_t level) {
  onChangeFadeSpeed(level);
}
