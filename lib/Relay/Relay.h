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

  void _on();
  void _off();
  void _checkCoil();
};
