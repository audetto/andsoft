#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace AndSoft {

  typedef std::vector<double> Array;

  template<typename T> inline T SQR(const T a) {
    return a * a;
  }

  inline __attribute__((noreturn)) void runtime_error(const std::string &msg) {
    throw msg;
  }

  inline void check_gsl_error(const int result) {
    if (result)
      throw result;
  }
}

#endif
