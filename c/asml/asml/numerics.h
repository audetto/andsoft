#ifndef ASI_NUMERICS_H
#define ASI_NUMERICS_H

#include <vector>

namespace ASI
{
    double decayIntegral(double x);    // f(0) = 1
    double decayIntegral2(double x);   // f(0) = -0.5

    double finiteDifference(const std::vector<double> & xa, const std::vector<double> & ya, double x, size_t order);

    void numerics_try();
}

#endif
