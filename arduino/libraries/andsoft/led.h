#pragma once

#include "Arduino.h"

namespace AndSoft
{

  class LED
  {
  public:
    LED(const uint8_t pin);

    void setup();

    void loop();

    void on();

    void off();

    void flash(unsigned long const * periods, const int size, const uint8_t first);

  private:

    const uint8_t myPIN;
    unsigned long const * myPeriods;
    int mySize;
    uint8_t myFirst;

    bool myActive;
    int myPosition;
    uint8_t myDigital;
    unsigned long myNextWakeupTime;
  };

}

// Local Variables:
// mode: c++
// End:
