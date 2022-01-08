# led-panel-controller

WiFi controller for LED panel fixtures.

**NOTE: this project is still at its initial development stage.**

## usage

### web interface

Each instance of the controller exposes a web interface.

The following examples assume a controller configured as `led-panel-1.local`.

At `http://led-panel-1.local/` a web page is returned with some basic controls.

Besides that, the controller accepts a few `GET` routes:
* `http://led-panel-1.local/on`: Turn the light on at maximum value (same as `max`).
* `http://led-panel-1.local/off`: Turn the light off.
* `http://led-panel-1.local/lo`: Turn the light on at minimum value.
* `http://led-panel-1.local/med`: Turn the light on at medium value.
* `http://led-panel-1.local/hi`: Turn the light on at maximum value (same as `on`).
* `http://led-panel-1.local/level?value=75`: Turn the light on at 75%.
* `http://led-panel-1.local/default?value=50`: Set the default level (for startup) at 50%.

## setup

Once the project is cloned, copy `lib/Config/Config.sample.h` as `lib/Config/Config.h` and edit it to set your configuration.

### config.h
```c
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

// Relay coil pins
#define RELAY_SET_PIN (4)
#define RELAY_RST_PIN (5)

// Relay coil activation time(ms) (see relay datasheet)
#define RELAY_COIL_ACTIVATION_TIME (40)

// PWM DIM pin
#define PWM_DIM_PIN (14)

// PWM settings
#define PWM_MIN_VALUE (26)
#define PWM_MAX_VALUE (255)

// DEMO Jumper pin
#define DEMO_JUMPER_PIN (13)
```


* `WIFI_SSID`: The name of your WiFi network.
* `WIFI_PASSWORD`: Password to connect to the network.
* `NETWORK_CONNECTION_CHECK_DELAY`: While connected, the program will make sure connection is up. This is the delay in milliseconds.
* `PANEL_ID`: Id of the panel (alphanumeric).
* `PANEL_DEFAULT_LEVEL`: The default light level for this panel (can be changed later).
* `MDNS_NAME`: Name for this device in the network.
* `MDNS_NETWORK`: Local network name.
* `WEB_SERVER_PORT`: TCP port for the web server.
* `RELAY_SET_PIN`: `ESP8266 GPIO` pin to use for the relay's `SET` coil.
* `RELAY_RST_PIN`: `ESP8266 GPIO` pin to use for the relay's `RESET` coil.
* `RELAY_COIL_ACTIVATION_TIME`: time (ms) to keep the relay's coil activated (see relay's datasheet).
* `PWM_DIM_PIN`: `ESP8266 GPIO` pin to use for the `PWM` dimmer.
* `PWM_MIN_VALUE`: Minimum `PWM` value accepted by the LED Panel driver.
* `PWM_MAX_VALUE`: Maximum `PWM` value accepted by the LED Panel driver.
* `DEMO_JUMPER_PIN`: `ESP8266 GPIO` pin to use for the `demo mode`.
