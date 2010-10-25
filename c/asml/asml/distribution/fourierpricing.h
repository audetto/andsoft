#ifndef ASI_FOURIER_PRICING_H
#define ASI_FOURIER_PRICING_H

#include <asml/utils/utils.h>
#include <asml/distribution/distribution.h>

#include <vector>

namespace ASI
{
    void FFT_Real(std::vector<double> & data, bool direct);
    void FFT_Unpack(const std::vector<double> & real, std::vector<cpl> & comple);
    void FFT_Pack(const std::vector<cpl> & comple, std::vector<double> & real);

    // strikes are spaced using sigmaSqrtT = sqrt(varianceSwap(dist))
    void priceViaFFT(const size_t N, const double std_dev, const Distribution & dist, std::vector<double> & strikes, std::vector<double> & prices);
}

#endif
