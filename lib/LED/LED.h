#pragma once

class LED
{
public:
  LED();
  ~LED();

  void setup(uint8_t pin, bool inverse=false);
  void loop();

  void on();
  void off();

  void blink(uint32_t onTime, uint32_t offTime);

private:
  void _on();
  void _off();

  uint8_t miPin;
  bool mbInverseLogic;
  uint8_t miLowValue;
  uint8_t miHighValue;
  bool mbLEDIsOn;
  bool mbBlinking;
  uint32_t miOnTime;
  uint32_t miOffTime;
  uint32_t miLastLEDStateChange;
};
