#ifndef UTILS_H
#define UTIlS_H

#include <string>

namespace AndSoft {

  template<typename T> inline T SQR(T a) {
    return a * a;
  }

  inline __attribute__((noreturn)) void runtime_error(const std::string &msg) {
    throw msg;
  }

}

#endif
