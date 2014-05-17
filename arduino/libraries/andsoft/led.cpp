#include "led.h"

namespace AndSoft
{

  LED::LED(const uint8_t pin)
    : myPIN(pin)
    , myPeriods(NULL)
    , mySize(0)
    , myFirst(LOW)
    , myActive(false)
  {
  }

  void LED::setup()
  {
    pinMode(myPIN, OUTPUT);
  }

  void LED::loop()
  {
    if (myActive)
    {
      const unsigned long now = millis();
      if (now >= myNextWakeupTime)
      {
	++myPosition;
	if (myPosition == mySize)
	{
	  myPosition = 0;
	  myDigital = myFirst;
	}
	else
	{
	  myDigital = myDigital == HIGH ? LOW : HIGH;
	}
	digitalWrite(myPIN, myDigital);
	myNextWakeupTime += myPeriods[myPosition];
      }
    }
  }

  void LED::on()
  {
    myActive = false;
    digitalWrite(myPIN, HIGH);
  }

  void LED::off()
  {
    myActive = false;
    digitalWrite(myPIN, LOW);
  }

  void LED::flash(unsigned long const * periods, const int size, const uint8_t first)
  {
    myActive = size > 1;
    digitalWrite(myPIN, first);

    if (myActive)
    {
      myPeriods = periods;
      mySize = size;
      myFirst = first;

      const unsigned long now = millis();
      myPosition = 0;
      myDigital = first;

      myNextWakeupTime = now + myPeriods[myPosition];
    }
  }


}
