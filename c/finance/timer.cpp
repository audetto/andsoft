#include "timer.h"

namespace AndSoft {

  double Timer::elapsed() const {
    const ticks end = getticks();
    return ::elapsed(myStart, end);
  }

}
