#include <asi/fourierpricing.h>

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

}
