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

// MQTT settings
#define MQTT_SERVER "my-mqtt-server.local"
#define MQTT_PORT  (1883)
#define MQTT_RECONNECT_DELAY  (1000)
#define MQTT_STATUS_TOPIC "status"

// OTA settings
#define OTA_PORT (8266)
#define OTA_PASSWORD "my-ota-password"

// Relay coil pins
#define RELAY_SET_PIN (4) // 4 = D2
#define RELAY_RST_PIN (5) // 5 = D1

// Relay coil activation time(ms) (see relay datasheet)
#define RELAY_COIL_ACTIVATION_TIME (40)

// Relay "throttling" (avoiding too many fast changes)
#define RELAY_FASTEST_CHANGE (1000)

// PWM DIM pin
#define PWM_DIM_PIN (14) // 14 = D5

// PWM settings
#define PWM_ON_THRESHOLD (179)
#define PWM_DEFAULT_FADE_SPEED (5)

// PWM "throttling" (avoiding too many fast changes)
#define PWM_FASTEST_CHANGE (50)

// DEMO Jumper pin
#define DEMO_JUMPER_PIN (15) // 15 = D8
