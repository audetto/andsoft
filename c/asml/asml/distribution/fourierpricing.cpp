#include <asml/distribution/fourierpricing.h>
#include <asml/distribution/distribution.h>
#include <asml/analytics/implied.h>
#include <asml/utils/error.h>

#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>

namespace
{
    using namespace ASI;
    using namespace std;

    /* Those 2 functions should exist in gsl but I cannot find them */

    void asi_fft_real_pack(const vector<cpl> & input, vector<double> & output)
    {
	const size_t N = input.size();
	const size_t NR = 2 * (N - 1);
	output.resize(NR);

	output[0] = input[0].real();

	for (size_t i = 1; i < NR / 2; ++i)
	{
	    output[i]      = input[i].real();
	    output[NR - i] = input[i].imag();
	}

	output[NR / 2] = input[NR / 2].real();
    }

    void asi_fft_real_unpack(const vector<double> & input, vector<cpl> & output)
    {
	const size_t N = input.size();
	output.resize(N);

	output[0] = cpl(input[0], 0.0);

	for (size_t i = 1; i < N / 2; ++i)
	{
	    output[i]      = cpl(input[i],  input[N - i]);
	    output[N - i]  = cpl(input[i], -input[N - i]);
	}

	output[N / 2] = cpl(input[N / 2], 0.0);
    }

    /* This is the method proposed by Carr/Madan */

    cpl characteristicOfOTMCall(const double u, const Distribution & dist)
    {
	if (u == 0.0)
	{
	    const cpl result = dist.derivative(1.0);
	    return result;
	}

	const cpl arg = u - I;
	const cpl lt = exp(dist.laplaceExponent(I * arg));
	const cpl result = (lt - 1.0) / (I * u - u * u);
	return result;
    }

}

namespace ASI
{
    using namespace std;

    void FFT_Real(vector<double> & data, bool direct)
    {
	if (direct)
	    gsl_fft_real_radix2_transform (&data[0], 1, data.size());
	else
	    gsl_fft_halfcomplex_radix2_inverse (&data[0], 1, data.size());
    }

    void FFT_Unpack(const vector<double> & real, vector<cpl> & comple)
    {
	asi_fft_real_unpack (real, comple);
    }

    void FFT_Pack(const vector<cpl> & comple, vector<double> & real)
    {
	asi_fft_real_pack (comple, real);
    }

    void priceViaFFT(const size_t N, const double std_dev, const Distribution & dist, vector<double> & strikes, vector<double> & prices)
    {
	if (!isPowerOfTwo(N))
	  THROW_EXCEPTION(N << " is not a power of 2!");

	const double varSwap = varianceSwap(dist);
	const double sigmaSqrtT = sqrt(varSwap);
	const double equivalentSigmaSqrtT = sigmaSqrtT;

	BlackScholes bs(equivalentSigmaSqrtT);

	const double dk = 2.0 * std_dev * sigmaSqrtT / N;
	const double df = 2.0 * M_PI / (N * dk);
	const double mink = -std_dev * sigmaSqrtT;

	vector<cpl> calls(N / 2 + 1);
	for (size_t i = 0; i < N / 2 + 1; ++i)
	{
	    const double f = -(i * df); // gsl and carr/madan don't agree on the sign of the exponent of the transform
	    calls[i] = (characteristicOfOTMCall(f, dist) - characteristicOfOTMCall(f, bs)) / dk;
	}

	vector<double> packed;
	asi_fft_real_pack(calls, packed);
	gsl_fft_halfcomplex_radix2_inverse (&packed[0], 1, N);

	strikes.resize(N);
	prices.resize(N);
	for (size_t i = 0; i < N / 2; ++i)
	{
	    const double logK1 = i * dk;
	    strikes[N / 2 + i] = exp(logK1);
	    prices [N / 2 + i] = packed[i] + blackCallPrice(logK1, equivalentSigmaSqrtT);

	    const double logK2 = mink + i * dk;
	    strikes[i] = exp(logK2);
	    prices [i] = packed[N / 2 + i] + blackCallPrice(logK2, equivalentSigmaSqrtT);
	}
    }
}
