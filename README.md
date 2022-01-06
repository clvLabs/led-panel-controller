# led-panel-controller

WiFi controller for LED panel fixtures.

## setup

Once the project is cloned, copy `src/resources/config.sample.h` as `src/resources/config.h` and edit it to set your configuration.

### config.h
```c
// config.h
// led-panel-controller settings

// WiFi settings
#define WIFI_SSID      "my-wifi-network"
#define WIFI_PASSWORD  "my-password"

// ID of the LED panel (for multiple panel setups)
#define LEDPANELID  "1"

// MDNS settings
#define MDNSNAME    "led-panel-" LEDPANELID
#define MDNSNETWORK "local"
```


#### WiFi settings
* `WIFI_SSID`: The name of your WiFi network.
* `WIFI_PASSWORD`: Password to connect to the network.

#### ID of the LED panel (for multiple panel setups)
* `LEDPANELID`: Id of the panel (alphanumeric).

#### MDNS settings
* `MDNSNAME`: Name for this device in the network.
* `MDNSNETWORK`: Local network name.
