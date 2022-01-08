#include <Arduino.h>
#include "WebServer.h"
#include "Config.h"
#include "resources/index.css.h"

WebServer::WebServer()
: onChangeLevel(nullptr)
{}

WebServer::~WebServer()
{}

void WebServer::start() {
  mServer.onNotFound(std::bind(&WebServer::handleNotFound, this));
  mServer.on("/index.css", std::bind(&WebServer::handleIndexCSS, this));

  mServer.on("/", std::bind(&WebServer::handleHome, this));
  mServer.on("/reboot", std::bind(&WebServer::handleReboot, this));

  mServer.on("/on", std::bind(&WebServer::handlePresetLevel,  this, 100));
  mServer.on("/off", std::bind(&WebServer::handlePresetLevel, this, 0));
  mServer.on("/lo", std::bind(&WebServer::handlePresetLevel,  this, 1));
  mServer.on("/med", std::bind(&WebServer::handlePresetLevel, this, 50));
  mServer.on("/hi", std::bind(&WebServer::handlePresetLevel,  this, 100));

  mServer.on("/level", std::bind(&WebServer::handleLevel, this));
  mServer.on("/default", std::bind(&WebServer::handleDefault, this));

  mServer.begin(WEB_SERVER_PORT);
  Serial.print("Web server started on port ");
  Serial.println(WEB_SERVER_PORT);
}

void WebServer::loop() {
  mServer.handleClient();
}

void WebServer::sendRedirect() {
  String page = "";
  page += "<head>";
  page += "<meta http-equiv=\"Refresh\" content=\"0; URL=/\">";
  page += "</head>";
  mServer.send(200, "text/html", page);
}

void WebServer::handleHome() {
  String page = "";

  page += "<head>";
  page += "<link rel=stylesheet type=text/css href=\"/index.css\">";
  page += "</head>";
  page += "<body>";
  page += "<h1>led-panel-controller</h1>";
  page += "<h2>LED panel #" PANEL_ID "</h2>";
  page += MDNS_NAME "." MDNS_NETWORK;
  // page += MDNS_NAME "." MDNS_NETWORK " @ " + network.localIP().toString();
  // page += "<p>";
  // page += "Active LED mode: <b>";
  // page += pwmMode;
  // page += "</b>";
  page += "<p>";

  page += "<button type='button' title='ON' onclick='window.location = \"/on\";'>ON</button>";
  page += "<button type='button' title='OFF' onclick='window.location = \"/off\";'>OFF</button>";
  page += "<button type='button' title='lo' onclick='window.location = \"/lo\";'>lo</button>";
  page += "<button type='button' title='med' onclick='window.location = \"/med\";'>med</button>";
  page += "<button type='button' title='hi' onclick='window.location = \"/hi\";'>hi</button>";

  page += "</body>";
  mServer.send(200, "text/html", page);
}

void WebServer::handleReboot() {
  Serial.println("WARNING: Rebooting as requested via web.");
  ESP.reset();
}

void WebServer::handlePresetLevel(uint8_t level) {
  if (onChangeLevel)
    onChangeLevel(level);
  sendRedirect();
}

void WebServer::handleLevel() {
  for (uint8_t i = 0; i < mServer.args(); i++) {
    if (mServer.argName(i) == "value") {
      if (onChangeLevel) {
        uint8_t level = (uint8_t) mServer.arg(i).toInt();
        onChangeLevel(level);
      }
      sendRedirect();
      return;
    }
  }

  mServer.send(400, "text/plain", "Bad request");
}

void WebServer::handleDefault() {
  for (uint8_t i = 0; i < mServer.args(); i++) {
    if (mServer.argName(i) == "value") {
      if (onChangeDefault) {
        uint8_t level = (uint8_t) mServer.arg(i).toInt();
        onChangeDefault(level);
      }
      sendRedirect();
      return;
    }
  }

  mServer.send(400, "text/plain", "Bad request");
}

void WebServer::handleNotFound() {
  mServer.send(404, "text/plain", "Not found");
}

void WebServer::handleIndexCSS() {
  mServer.send(200, "text/css", INDEX_CSS);
}
