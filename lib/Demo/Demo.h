#pragma once
#include "State.h"

class Demo
{
public:
  std::function< void() > onDemoStarted;
  std::function< void(uint8_t) > onChangeLevel;

  Demo();
  ~Demo();

  void start(State* state);
  void loop();

  bool isActive();

private:
  State* mState;
  uint8_t miPin;
  bool mbActive;

  unsigned long miNextDemoCheck;
  unsigned long miNextDemoRSSI;
  uint8_t miDemoPhase;
  uint8_t miDemoIteration;
  bool mbDemoRampingUp;
};
