#pragma once

class Relay
{
public:
  Relay();
  ~Relay();

  void loop();

  void setState(bool on);

private:
  uint8_t miSetPin;
  uint8_t miRstPin;
  bool mbOn;
};
