#pragma once

class Dimmer
{
public:
  Dimmer();
  ~Dimmer();

  void loop();

  void setLevel(unsigned short level);

private:
  int miPin;
  unsigned short miLevel;
};
