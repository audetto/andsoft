#pragma once

namespace AndSoft
{

  class Timer
  {
  public:
    Timer();

    void setup();

    void loop();

    void start(const unsigned long mill);

    // true is the timer has expired
    // (or never started)
    bool done() const;

  private:

    bool myActive;
    unsigned long myNextWakeupTime;
  };

}

// Local Variables:
// mode: c++
// End:
