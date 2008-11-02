#ifndef ASI_FOURIER_PRICING_H
#define ASI_FOURIER_PRICING_H

#include <asi/utils.h>

#include <vector>

namespace ASI
{
    void FFT_Real(std::vector<double> & data, bool direct);
    void FFT_Unpack(const std::vector<double> & real, std::vector<cpl> & comple);
    void FFT_Pack(const std::vector<cpl> & comple, std::vector<double> & real);
}

#endif
