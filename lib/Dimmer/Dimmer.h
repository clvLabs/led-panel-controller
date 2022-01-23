#pragma once

class Dimmer
{
public:
  Dimmer();
  ~Dimmer();

  void start();
  void loop();

  void setLevel(uint8_t level);
  void fade(uint8_t level, uint8_t speed);

private:
  uint8_t miPin;
  uint8_t miLevel;
  uint8_t miFadeLevel;
  uint8_t miFadeSpeed;
  unsigned long miLastPWMChange;
  bool mbThrottledValueWaiting;
  bool mbThrottledWaitingFade;
  uint8_t miThrottledValue;
  uint8_t miThrottledSpeed;

  void _updatePWM(uint8_t level);
};
