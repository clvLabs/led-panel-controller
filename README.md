# led-panel-controller

WiFi controller for LED panel fixtures.

**NOTE: this project is still at its initial development stage.**

## Usage

### Web interface

Each instance of the controller exposes a web interface.

The following examples assume a controller configured as `led-panel-1.local`.

* At `http://led-panel-1.local/` you'll find some basic controls.
* At `http://led-panel-1.local/info` you'll find status info.

Besides that, the controller accepts a few `GET` routes:
* `http://led-panel-1.local/do/on`: Turn the light on at maximum value (same as `max`).
* `http://led-panel-1.local/do/off`: Turn the light off.
* `http://led-panel-1.local/do/lo`: Turn the light on at minimum value.
* `http://led-panel-1.local/do/med`: Turn the light on at medium value.
* `http://led-panel-1.local/do/hi`: Turn the light on at maximum value (same as `on`).
* `http://led-panel-1.local/do/reboot`: Reboot the module.
* `http://led-panel-1.local/set/level?value=75`: Turn the light on at 75%.
* `http://led-panel-1.local/set/default?value=50`: Set the default level (for startup) at 50%.

### MQTT

The device will expose its state in the `MDNS_NAME/MQTT_STATUS_TOPIC` (e.g. `led-panel-1/status`) topic as a `json` object:
```json
 {
    "lightLevel": {
      "current": 75,
      "default": 100,
    },
    "network": {
      "rssi": -59
    },
    "misc": {
      "uptime": 120
    }
 }
```
* `lightLevel`: light dimmer status
  * `current`: current level
  * `default`: default level
* `network`: network connection status
  * `rssi`: current RSSI value
* `misc`: miscellaneous status
  * `uptime`: number of seconds the board has been `ON` (overflows/resets about every 49 days)


## Setup

This project is built using [PlatformIO](https://platformio.org/), but I'm sure you can build it with the vanilla Arduino IDE and some patience if you want to :)

Once the project is cloned, copy the template files and edit them to set your configuration.

```
$ cp lib/Config/Config.sample.h lib/Config/Config.h
$ cp platformio.sample.ini platformio.ini
```

### config.h
* `WIFI_SSID`: The name of your WiFi network.
* `WIFI_PASSWORD`: Password to connect to the network.
* `NETWORK_CONNECTION_CHECK_DELAY`: While connected, the program will make sure connection is up. This is the delay in milliseconds.
* `PANEL_ID`: Id of the panel (alphanumeric).
* `PANEL_DEFAULT_LEVEL`: The default light level for this panel (can be changed later).
* `MDNS_NAME`: Name for this device in the network.
* `MDNS_NETWORK`: Local network name.
* `WEB_SERVER_PORT`: TCP port for the web server.
* `MQTT_SERVER`: MQTT server.
* `MQTT_PORT`: MQTT port.
* `MQTT_RECONNECT_DELAY`: Delay (ms) between `MQTT` reconnection attempts.
* `*MQTT_STATUS_TOPIC`: Topic for the status messages.
* `OTA_PORT`: UDP port for [ArduinoOTA](https://www.arduino.cc/reference/en/libraries/arduinoota/).
* `OTA_PASSWORD`: Password for OTA updates.
* `RELAY_SET_PIN`: `ESP8266 GPIO` pin to use for the relay's `SET` coil.
* `RELAY_RST_PIN`: `ESP8266 GPIO` pin to use for the relay's `RESET` coil.
* `RELAY_COIL_ACTIVATION_TIME`: time (ms) to keep the relay's coil activated (see relay's datasheet).
* `PWM_DIM_PIN`: `ESP8266 GPIO` pin to use for the `PWM` dimmer.
* `PWM_ON_THRESHOLD`: Maximum `PWM` value that makes the LED Panel driver turn the light ON (see comment in `Dimmer.cpp`).
* `DEMO_JUMPER_PIN`: `ESP8266 GPIO` pin to use for the `demo mode`.


### platformio.ini
* `upload_port`: your device's `MDNS` full name (based on `MDNS_NAME` / `PANEL_ID` / `MDNS_NETWORK`).
* `upload_flags:`:
  * `--port`: set to the same value as `OTA_PORT`.
  * `--auth`: set to the same value as `OTA_PASSWORD`.
