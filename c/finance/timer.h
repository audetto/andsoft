#ifndef TIMER_H
#define TIMER_H

#include "cycle.h"

namespace AndSoft {

  class Timer {
  public:
    Timer() : myStart(getticks()) { }
    double elapsed() const;
  private:
    const ticks myStart;
  };

}

#endif
