#ifndef ASI_HESTON_H
#define ASI_HESTON_H

#include <asi/utils.h>

namespace ASI
{
    double hestonCallPrice(const double strike, const double time, const double sigma, const double kappa, const double theta, const double alpha, const double rho, size_t n);
    double blackCallPrice(const double strike, const double time, const double sigma);

    void heston_try();
}

#endif
