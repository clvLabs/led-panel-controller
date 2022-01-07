# led-panel-controller

WiFi controller for LED panel fixtures.

## setup

Once the project is cloned, copy `lib/Config/Config.sample.h` as `lib/Config/Config.h` and edit it to set your configuration.

### config.h
```c
// config.h
// led-panel-controller settings

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

// Relay coil pins
#define RELAY_SET_PIN (4)
#define RELAY_RST_PIN (5)

// PWM DIM pin
#define PWM_DIM_PIN (0)
```


#### WiFi settings
* `WIFI_SSID`: The name of your WiFi network.
* `WIFI_PASSWORD`: Password to connect to the network.

#### ID of the LED panel (for multiple panel setups)
* `LEDPANEL_ID`: Id of the panel (alphanumeric).

#### MDNS settings
* `MDNS_NAME`: Name for this device in the network.
* `MDNS_NETWORK`: Local network name.
