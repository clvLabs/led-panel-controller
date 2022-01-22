// config.h
// led-panel-controller settings
#pragma once

// WiFi settings
#define WIFI_SSID      "my-wifi-network"
#define WIFI_PASSWORD  "my-password"

// Delay(ms) between network connectivity checks (once connected)
#define NETWORK_CONNECTION_CHECK_DELAY  (10000)

// ID of the LED panel (for multiple panel setups)
#define PANEL_ID  "1"

// Default light level for this panel at startup (0-100%)
#define PANEL_DEFAULT_LEVEL  (100)

// MDNS settings
#define MDNS_NAME    "led-panel-" PANEL_ID
#define MDNS_NETWORK "local"

// Web server settings
#define WEB_SERVER_PORT (80)

// OTA settings
#define OTA_PORT (8266)
#define OTA_PASSWORD "my-ota-password"

// Relay coil pins
#define RELAY_SET_PIN (4)
#define RELAY_RST_PIN (5)

// Relay coil activation time(ms) (see relay datasheet)
#define RELAY_COIL_ACTIVATION_TIME (40)

// PWM DIM pin
#define PWM_DIM_PIN (14)

// PWM settings
#define PWM_ON_THRESHOLD (179)

// DEMO Jumper pin
#define DEMO_JUMPER_PIN (13)
