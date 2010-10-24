#ifndef ASI_DISTRIBUTION_H
#define ASI_DISTRIBUTION_H

#include <asml/utils.h>

namespace ASI
{
    class Distribution
    {
    public:
	virtual ~Distribution();

	/*
	  E(e^(x * X)) = e^LE(x)
	  x in C
	  LE(0) = 0
	  LE(1) = 1 when X is Log(S)
	 */

	virtual cpl laplaceExponent(const cpl & x) const = 0;
	virtual cpl derivative(double x) const;
    };

    class BlackScholes : public Distribution
    {
    public:
	BlackScholes(double aSigmaSqrtT);

	virtual cpl laplaceExponent(const cpl & x) const;
    private:
	const double sigmaSqrtT;
    };

    double variance(const Distribution & dist);
    double varianceSwap(const Distribution & dist);
}

#endif
