#include <asml/distribution.h>

namespace ASI
{
    Distribution::~Distribution()
    {
    }

    cpl Distribution::derivative(double x) const
    {
	const cpl x_down(x - 0.00001, 0.0);
	const cpl x_up  (x + 0.00001, 0.0);

	const cpl f_down = laplaceExponent(x_down);
	const cpl f_up   = laplaceExponent(x_up);

	const cpl value = (f_up - f_down) / (x_up - x_down);
	return value;
    }

    BlackScholes::BlackScholes(double aSigmaSqrtT) : sigmaSqrtT(aSigmaSqrtT)
    {
    }
	
    cpl BlackScholes::laplaceExponent(const cpl & x) const
    {
	const cpl drift     = -0.5 * sigmaSqrtT * sigmaSqrtT * x;
	const cpl diffusion = 0.5 * sigmaSqrtT * sigmaSqrtT * x * x;
	return drift + diffusion;
    }

    double variance(const Distribution & dist)
    {
	const double mean   = exp(dist.laplaceExponent(1.0)).real();
	const double second = exp(dist.laplaceExponent(2.0)).real();
	const double var = second - mean * mean;
	return var;
    }

    double varianceSwap(const Distribution & dist)
    {
	return -2.0 * dist.derivative(0.0).real();
    }
}
