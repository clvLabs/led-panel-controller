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
  mServer.on("/", std::bind(&WebServer::handleRoot, this));
  mServer.on("/index.css", std::bind(&WebServer::handleIndexCSS, this));
  mServer.on("/normal", std::bind(&WebServer::handleNormal, this));
  mServer.on("/fast", std::bind(&WebServer::handleFast, this));
  mServer.on("/slow", std::bind(&WebServer::handleSlow, this));
  mServer.onNotFound(std::bind(&WebServer::handleNotFound, this));

  mServer.begin(WEB_SERVER_PORT);
  Serial.print("- Web server started on port ");
  Serial.println(WEB_SERVER_PORT);
}

void WebServer::loop() {
  mServer.handleClient();
}

void WebServer::sendResultPage() {
  String page = "";

  page += "<head>";
  page += "<link rel=stylesheet type=text/css href=\"/index.css\">";
  page += "</head>";
  page += "<body>";
  page += "<h1>led-panel-controller</h1>";
  page += "<h2>LED panel #" LEDPANEL_ID "</h2>";
  // page += MDNS_NAME "." MDNS_NETWORK " @ " + network.localIP().toString();
  page += "<p>";
  page += "Active LED mode: <b>";
  // page += pwmMode;
  page += "</b>";
  page += "<p>";

  page += "<button type='button' title='slow' onclick='window.location = \"/slow\";'>slow</button>";
  page += "<button type='button' title='normal' onclick='window.location = \"/normal\";'>normal</button>";
  page += "<button type='button' title='fast' onclick='window.location = \"/fast\";'>fast</button>";

  page += "</body>";
  mServer.send(200, "text/html", page);
}

void WebServer::sendRedirect() {
  String page = "";
  page += "<head>";
  page += "<meta http-equiv=\"Refresh\" content=\"0; URL=/\">";
  page += "</head>";
  mServer.send(200, "text/html", page);
}

void WebServer::handleRoot() {
  sendResultPage();
}

void WebServer::handleFast() {
  // setPWMMode_Fast();
  if (onChangeLevel)
    onChangeLevel(PWM_MAX_VALUE);
  sendRedirect();
}

void WebServer::handleNormal() {
  // setPWMMode_Normal();
  if (onChangeLevel)
    onChangeLevel(80);
  sendRedirect();
}

void WebServer::handleSlow() {
  // setPWMMode_Slow();
  if (onChangeLevel)
    onChangeLevel(PWM_MIN_VALUE);
  sendRedirect();
}

void WebServer::handleIndexCSS() {
  mServer.send(200, "text/css", INDEX_CSS);
}

void WebServer::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += mServer.uri();
  message += "\nMethod: ";
  message += (mServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += mServer.args();
  message += "\n";
  for (uint8_t i = 0; i < mServer.args(); i++) {
    message += " " + mServer.argName(i) + ": " + mServer.arg(i) + "\n";
  }
  mServer.send(404, "text/plain", message);
}
