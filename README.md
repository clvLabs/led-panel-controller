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
* `http://led-panel-1.local/reboot`: Reboot the module.

## setup

Once the project is cloned, copy the template files and edit them to set your configuration.

```
$ cp lib/Config/Config.sample.h lib/Config/Config.h
$ cp platformio.sample.ini platformio.ini
```

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
```

* `WIFI_SSID`: The name of your WiFi network.
* `WIFI_PASSWORD`: Password to connect to the network.
* `NETWORK_CONNECTION_CHECK_DELAY`: While connected, the program will make sure connection is up. This is the delay in milliseconds.
* `PANEL_ID`: Id of the panel (alphanumeric).
* `PANEL_DEFAULT_LEVEL`: The default light level for this panel (can be changed later).
* `MDNS_NAME`: Name for this device in the network.
* `MDNS_NETWORK`: Local network name.
* `WEB_SERVER_PORT`: TCP port for the web server.
* `OTA_PORT`: UDP port for [ArduinoOTA](https://www.arduino.cc/reference/en/libraries/arduinoota/).
* `OTA_PASSWORD`: Password for OTA updates.
* `RELAY_SET_PIN`: `ESP8266 GPIO` pin to use for the relay's `SET` coil.
* `RELAY_RST_PIN`: `ESP8266 GPIO` pin to use for the relay's `RESET` coil.
* `RELAY_COIL_ACTIVATION_TIME`: time (ms) to keep the relay's coil activated (see relay's datasheet).
* `PWM_DIM_PIN`: `ESP8266 GPIO` pin to use for the `PWM` dimmer.
* `PWM_ON_THRESHOLD`: Maximum `PWM` value that makes the LED Panel driver turn the light ON (see comment in `Dimmer.cpp`).
* `DEMO_JUMPER_PIN`: `ESP8266 GPIO` pin to use for the `demo mode`.


### platformio.ini
```ini
[env:esp12e]
platform = espressif8266
board = esp12e
framework = arduino
monitor_speed = 115200
upload_protocol = espota
upload_port = led-panel-1.local
upload_flags =
    --port=8266
    --auth=my-ota-password
```

* `upload_port`: your device's `MDNS` full name (based on `MDNS_NAME` / `PANEL_ID` / `MDNS_NETWORK`).
* `upload_flags:`:
  * `--port`: set to the same value as `OTA_PORT`.
  * `--auth`: set to the same value as `OTA_PASSWORD`.
