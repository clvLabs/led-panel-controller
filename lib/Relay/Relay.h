#pragma once

class Relay
{
public:
  Relay();
  ~Relay();

  void loop();

  void setState(bool on);

private:
  int miSetPin;
  int miRstPin;
  bool mbOn;
};
