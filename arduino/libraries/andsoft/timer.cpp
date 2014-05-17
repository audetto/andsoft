#include "timer.h"
#include "Arduino.h"

namespace AndSoft
{

  Timer::Timer()
    : myActive(false)
  {
  }

  void Timer::setup()
  {
  }

  void Timer::loop()
  {
    if (myActive)
    {
      const unsigned long now = millis();
      myActive = now < myNextWakeupTime;
    }
  }

  void Timer::start(const unsigned long mill)
  {
    const unsigned long now = millis();
    myActive = true;
    myNextWakeupTime = now + mill;
  }

  bool Timer::done() const
  {
    return !myActive;
  }

}
