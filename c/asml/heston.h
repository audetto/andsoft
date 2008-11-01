#ifndef ASI_HESTON_H
#define ASI_HESTON_H

#include <asi/utils.h>

namespace ASI
{
    double hestonCallPrice(const double strike, const double time1, const double time2, const double sigma, const double kappa, const double theta, const double alpha, const double rho, size_t n);

    void heston_try();
}

#endif
