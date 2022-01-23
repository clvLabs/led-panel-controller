#pragma once

class Relay
{
public:
  Relay();
  ~Relay();

  void start();
  void loop();

  void on();
  void off();

private:
  uint8_t miSetPin;
  uint8_t miRstPin;
  bool mbOn;
  bool mbCoilActive;
  uint32_t miCoilActivationTime;
  unsigned long miLastStateChange;
  bool mbThrottledValueWaiting;
  bool mbThrottledValue;

  void _setState(bool state);
  void _checkCoil();
};
