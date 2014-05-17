#include "button.h"

namespace AndSoft
{
  const unsigned long Button::ourGap = 20;

  Button::Button(const uint8_t pin)
    : myPIN(pin)
    , myStatus(HIGH) // PULLUP is the other way round
    , myNextValidTime(0)
    , myPressed(false)
  {
  }

  void Button::setup()
  {
    pinMode(myPIN, INPUT_PULLUP);
  }

  void Button::loop()
  {
    const unsigned long now = millis();
    if (now > myNextValidTime)
    {
      // ok, we have skipped the noise
      const uint8_t status = digitalRead(myPIN);
      if (status != myStatus)
      {
	// something has changed
	myStatus = status;
	myNextValidTime = now + ourGap;

	// mark button pressed
	myPressed = status == LOW;
      }
    }
  }

  uint8_t Button::read() const
  {
    return myStatus;
  }

  bool Button::pressed()
  {
    const bool res = myPressed;

    // we only flag the "pressed" event once
    myPressed = false;
    return res;
  }

}
