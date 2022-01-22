#include <Arduino.h>
#include "WebServer.h"
#include "Config.h"
#include "resources/index.css.h"

WebServer::WebServer()
: onChangeLevel(nullptr)
, onChangeDefault(nullptr)
{}

WebServer::~WebServer()
{}

void WebServer::start(State* state) {
  mState = state;

  mServer.onNotFound(std::bind(&WebServer::handleNotFound, this));
  mServer.on("/index.css", std::bind(&WebServer::handleIndexCSS, this));

  mServer.on("/", std::bind(&WebServer::handleHome, this));
  mServer.on("/info", std::bind(&WebServer::handleInfo, this));

  mServer.on("/do/on", std::bind(&WebServer::handlePresetLevel,  this, "/do/on", 100));
  mServer.on("/do/off", std::bind(&WebServer::handlePresetLevel, this, "/do/off", 0));
  mServer.on("/do/lo", std::bind(&WebServer::handlePresetLevel,  this, "/do/lo", 1));
  mServer.on("/do/med", std::bind(&WebServer::handlePresetLevel, this, "/do/med", 50));
  mServer.on("/do/hi", std::bind(&WebServer::handlePresetLevel,  this, "/do/hi", 100));
  mServer.on("/do/reboot", std::bind(&WebServer::handleReboot, this));

  mServer.on("/set/level", std::bind(&WebServer::handleLevel, this));
  mServer.on("/set/default", std::bind(&WebServer::handleDefault, this));

  mServer.begin(mState->mWeb.miPort);
  Serial.print("[WebServer] Started on port ");
  Serial.println(mState->mWeb.miPort);
}

void WebServer::loop() {
  mServer.handleClient();
}

void WebServer::sendRedirect() {
  String page = "";
  page += "<head>";
  page += "<meta http-equiv=\"Refresh\" content=\"0; URL=/\">";
  page += "</head>";
  mServer.keepAlive(false);
  mServer.send(200, "text/html", page);
}

void WebServer::handleHome() {
  String page = "";

  page += "<head>";
  page += "<link rel=stylesheet type=text/css href=\"/index.css\">";
  page += "</head>";
  page += "<body>";

  page += "<h1>";
  page += mState->mNetwork.msMDNSName;
  page += ".";
  page += mState->mNetwork.msMDNSNetwork;
  page += "</h1>";

  page += "<button type='button' title='ON' onclick='window.location = \"/do/on\";'>ON</button>";
  page += "<button type='button' title='OFF' onclick='window.location = \"/do/off\";'>OFF</button>";
  page += "<button type='button' title='lo' onclick='window.location = \"/do/lo\";'>lo</button>";
  page += "<button type='button' title='med' onclick='window.location = \"/do/med\";'>med</button>";
  page += "<button type='button' title='hi' onclick='window.location = \"/do/hi\";'>hi</button>";

  page += "</body>";
  mServer.keepAlive(false);
  mServer.send(200, "text/html", page);
}

void WebServer::handleInfo() {
  String page = "";

  page += "<head>";
  page += "<link rel=stylesheet type=text/css href=\"/index.css\">";
  page += "</head>";
  page += "<body>";

  page += "<h1>";
  page += mState->mNetwork.msMDNSName;
  page += ".";
  page += mState->mNetwork.msMDNSNetwork;
  page += "</h1>";

  page += "<h2>LED Panel light level</h2><ul>";
  page += "<li>Current: <b>";
  page += mState->mLightLevel.miCurrent;
  page += "</b></li>";
  page += "<li>Default: <b>";
  page += mState->mLightLevel.miDefault;
  page += "</b></li>";
  page += "</ul>";

  page += "<h2>Network</h2><ul>";
  page += "<li>SSID: <b>";
  page += mState->mNetwork.msSSID;
  page += "</b></li>";

  page += "<li>RSSI: <b>";
  page += mState->mNetwork.miRSSI;
  page += "</b></li>";

  page += "<li>IP: <b>";
  page += mState->mNetwork.mIP.toString();
  page += "</b></li>";

  page += "<li>MAC: <b>";
  page += mState->mNetwork.msMAC;
  page += "</b></li>";

  page += "<li>MDNS: <b>";
  if (mState->mNetwork.mbMDNSStarted)
    page += "STARTED";
  else
    page += "NOT STARTED";
  page += "</b></li>";
  page += "</ul>";

  page += "</body>";
  mServer.keepAlive(false);
  mServer.send(200, "text/html", page);
}

void WebServer::handleReboot() {
  Serial.println("[WebServer] Received do/reboot - WARNING: Rebooting as requested.");
  Serial.println("WARNING: Rebooting as requested via web.");
  ESP.reset();
}

void WebServer::handlePresetLevel(const char* route, uint8_t level) {
  Serial.print("[WebServer] Received ");
  Serial.print(route);
  Serial.print(" - new level: ");
  Serial.println(level);
  if (onChangeLevel)
    onChangeLevel(level);
  sendRedirect();
}

void WebServer::handleLevel() {
  for (uint8_t i = 0; i < mServer.args(); i++) {
    if (mServer.argName(i) == "value") {
      Serial.print("[WebServer] Received set/level - payload: ");
      Serial.println(mServer.arg(i));
      if (onChangeLevel) {
        uint8_t level = (uint8_t) mServer.arg(i).toInt();
        onChangeLevel(level);
      }
      sendRedirect();
      return;
    }
  }

mServer.keepAlive(false);
  mServer.send(400, "text/plain", "Bad request");
}

void WebServer::handleDefault() {
  for (uint8_t i = 0; i < mServer.args(); i++) {
    if (mServer.argName(i) == "value") {
      Serial.print("[WebServer] Received set/default - payload: ");
      Serial.println(mServer.arg(i));
      if (onChangeDefault) {
        uint8_t level = (uint8_t) mServer.arg(i).toInt();
        onChangeDefault(level);
      }
      sendRedirect();
      return;
    }
  }

mServer.keepAlive(false);
  mServer.send(400, "text/plain", "Bad request");
}

void WebServer::handleNotFound() {
  mServer.keepAlive(false);
  mServer.send(404, "text/plain", "Not found");
}

void WebServer::handleIndexCSS() {
  mServer.keepAlive(false);
  mServer.send(200, "text/css", INDEX_CSS);
}
