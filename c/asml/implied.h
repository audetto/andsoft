#ifndef ASI_IMPLIED_H
#define ASI_IMPLIED_H

#include <asi/utils.h>
#include <vector>

namespace ASI
{
    std::vector<double> variousBS(double forward, double strike, double time, double sigma, double theta);
    double impliedVolatility(double price, double forward, double strike, double time, double theta);
    double blackCallPrice(double logStrike, double sigmaSqrtT);
}

#endif
