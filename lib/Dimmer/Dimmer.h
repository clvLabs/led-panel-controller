#pragma once

class Dimmer
{
public:
  Dimmer();
  ~Dimmer();

  void start();
  void loop();

  void setLevel(uint8_t level);

private:
  uint8_t miPin;
  uint8_t miLevel;
};
