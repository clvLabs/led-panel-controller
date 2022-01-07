// config.h
// led-panel-controller settings
#pragma once

// WiFi settings
#define WIFI_SSID      "my-wifi-network"
#define WIFI_PASSWORD  "my-password"

// ID of the LED panel (for multiple panel setups)
#define LEDPANEL_ID  "1"

// MDNS settings
#define MDNS_NAME    "led-panel-" LEDPANEL_ID
#define MDNS_NETWORK "local"

// Web server settings
#define WEB_SERVER_PORT (80)

// Status LED pin
#define STATUSLED_PIN (LED_BUILTIN)
#define STATUSLED_INVERTED (true)

// Relay coil pins
#define RELAY_SET_PIN (4)
#define RELAY_RST_PIN (5)

// PWM DIM pin
#define PWM_DIM_PIN (0)

// PWM settings
#define PWM_MIN_VALUE (26)
#define PWM_MAX_VALUE (255)

