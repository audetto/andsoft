#pragma once

#include "Arduino.h"

namespace AndSoft
{

  class Button
  {
  public:
    Button(const uint8_t pin);

    void setup();

    void loop();

    uint8_t read() const;

    // this will reset the flag as well
    bool pressed();

  private:

    static const unsigned long ourGap;

    const uint8_t myPIN;

    uint8_t myStatus;
    unsigned long myNextValidTime;
    bool myPressed;
  };

}

// Local Variables:
// mode: c++
// End:
