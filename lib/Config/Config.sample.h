// config.h
// led-panel-controller settings
#pragma once

// WiFi settings
#define WIFI_SSID      "my-wifi-network"
#define WIFI_PASSWORD  "my-password"

// ID of the LED panel (for multiple panel setups)
#define PANEL_ID  "1"

// Default light level for this panel at startup (0-100%)
#define PANEL_DEFAULT_LEVEL  (100)

// MDNS settings
#define MDNS_NAME    "led-panel-" PANEL_ID
#define MDNS_NETWORK "local"

// Web server settings
#define WEB_SERVER_PORT (80)

// Status LED pin
#define STATUSLED_PIN (12)
#define STATUSLED_INVERTED (false)

// Relay coil pins
#define RELAY_SET_PIN (4)
#define RELAY_RST_PIN (5)

// PWM DIM pin
#define PWM_DIM_PIN (14)

// PWM settings
#define PWM_MIN_VALUE (26)
#define PWM_MAX_VALUE (255)

// DEMO Jumper pin
#define DEMO_JUMPER_PIN (13)
