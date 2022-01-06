#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "resources/index.css.h"
#include "resources/config.h"


ESP8266WebServer server(80);

String ledMode = "";
unsigned long ledONTime = 0;
unsigned long ledOFFTime = 0;
const int ledPin = LED_BUILTIN;
bool ledIsON = false;
unsigned long ledLastChange = 0;

const int pwmPin = 14;
unsigned long pwmChangeTime = 0;
int pwmVal = 0;
bool pwmUp = true;
unsigned long pwmLastChange = 0;

void ledON() {
  digitalWrite(ledPin, LOW);
}

void ledOFF() {
  digitalWrite(ledPin, HIGH);
}

void setLEDMode_Fast() {
  Serial.println("New mode: FAST");
  ledMode = "FAST";
  ledONTime = 10;
  ledOFFTime = 90;
  pwmChangeTime = 2;
}

void setLEDMode_Normal() {
  Serial.println("New mode: NORMAL");
  ledMode = "NORMAL";
  ledONTime = 25;
  ledOFFTime = 3000;
  pwmChangeTime = 40;
}

void setLEDMode_Slow() {
  Serial.println("New mode: SLOW");
  ledMode = "SLOW";
  ledONTime = 25;
  ledOFFTime = 10000;
  pwmChangeTime = 250;
}

void sendResultPage() {
  String page = "";

  page += "<head>";
  page += "<link rel=stylesheet type=text/css href=\"/index.css\">";
  page += "</head>";
  page += "<body>";
  page += "<h1>led-panel-controller</h1>";
  page += "<h2>LED panel #" LEDPANELID "</h2>";
  page += MDNSNAME "." MDNSNETWORK " @ " + WiFi.localIP().toString();
  page += "<p>";
  page += "Active LED mode: <b>";
  page += ledMode;
  page += "</b>";
  page += "<p>";

  page += "<button type='button' title='slow' onclick='window.location = \"/slow\";'>slow</button>";
  page += "<button type='button' title='normal' onclick='window.location = \"/normal\";'>normal</button>";
  page += "<button type='button' title='fast' onclick='window.location = \"/fast\";'>fast</button>";

  page += "</body>";
  server.send(200, "text/html", page);
}

void sendRedirect() {
  String page = "";
  page += "<head>";
  page += "<meta http-equiv=\"Refresh\" content=\"0; URL=/\">";
  page += "</head>";
  server.send(200, "text/html", page);
}

void handleRoot() {
  sendResultPage();
}

void handleFast() {
  setLEDMode_Fast();
  sendRedirect();
}

void handleNormal() {
  setLEDMode_Normal();
  sendRedirect();
}

void handleSlow() {
  setLEDMode_Slow();
  sendRedirect();
}

void handleIndexCSS() {
  server.send(200, "text/css", INDEX_CSS);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void updateLED(void) {
  unsigned long ledElapsed = millis() - ledLastChange;

  unsigned long pwmElapsed = millis() - pwmLastChange;

  if (pwmElapsed > pwmChangeTime) {
    if (pwmUp) pwmVal++;
    else       pwmVal--;

    pwmLastChange = millis();

    if (pwmVal <= 0 || pwmVal >= 1023)
      pwmUp = !pwmUp;

    analogWrite(pwmPin, pwmVal);
  }


  if (ledIsON) {
    if (ledElapsed >= ledONTime) {
      ledIsON = false;
      ledOFF();
      ledLastChange = millis();
    }

  } else {
    if (ledElapsed >= ledOFFTime) {
      ledIsON = true;
      ledON();
      ledLastChange = millis();
    }

  }
}

const char* getWiFiStatusStr(wl_status_t status) {
  switch(status) {
    case WL_NO_SHIELD:        return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:      return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:    return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:   return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:        return "WL_CONNECTED";
    case WL_CONNECT_FAILED:   return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:  return "WL_CONNECTION_LOST";
    case WL_WRONG_PASSWORD:   return "WL_WRONG_PASSWORD";
    case WL_DISCONNECTED:     return "WL_DISCONNECTED";
    default:                  return "UNKNOWN";
  }
}

void startWiFi(void) {
  Serial.println(" Starting wifi");
  unsigned long startTime = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection
  wl_status_t status = WiFi.status();
  while (status != WL_CONNECTED) {
    Serial.print("  - WiFi status: ");
    Serial.println(getWiFiStatusStr(status));

    // Total waiting time: 500ms
    for (int i=0; i<3; i++) {
      ledON(); delay(50); ledOFF(); delay(50);
    }
    delay(200);

    status = WiFi.status();
  }
  unsigned long endTime = millis();

  Serial.print("- Connected to ");
  Serial.print(WIFI_SSID);
  Serial.print(" - IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("- Connection time (ms): ");
  Serial.println(endTime-startTime);

  if (MDNS.begin(MDNSNAME)) {
    Serial.println("- MDNS responder started");
  }
}

void startServer(void) {
  server.on("/", handleRoot);
  server.on("/index.css", handleIndexCSS);
  server.on("/normal", handleNormal);
  server.on("/fast", handleFast);
  server.on("/slow", handleSlow);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("- HTTP server started");
}

void setup(void) {
  ledLastChange = millis();
  pinMode(ledPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  ledOFF();

  Serial.begin(9600);
  delay(100);
  Serial.println("\n\n\n");
  Serial.println(" led-panel-controller - LED panel #" LEDPANELID);
  Serial.println("-------------------------------------");

  startWiFi();
  startServer();
  setLEDMode_Normal();

  Serial.println("- Listening");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  updateLED();
}
