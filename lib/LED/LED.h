#pragma once

class LED
{
public:
  LED();
  ~LED();

  void setup(int pin, bool inverse=false);
  void loop();

  void on();
  void off();

  void blink(unsigned long onTime, unsigned long offTime);

private:
  void _on();
  void _off();

  int miPin;
  bool mbInverseLogic;
  int miLowValue;
  int miHighValue;
  bool mbLEDIsOn;
  bool mbBlinking;
  unsigned long miOnTime;
  unsigned long miOffTime;
  unsigned long miLastLEDStateChange;
};
