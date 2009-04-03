#ifndef ASI_HESTON_H
#define ASI_HESTON_H

#include <asi/utils.h>
#include <asi/distribution.h>
#include <vector>

namespace ASI
{
    double hestonCallPrice(const double strike, const double time1, const double time2, const double sigma, const double kappa, const double theta, const double alpha, const double rho, size_t n);

    class Heston;

    class HestonDistribution : public Distribution
    {
    public:
	HestonDistribution(const double asigma, const double akappa, const double atheta, const double aalpha, const double arho, const double at1, const double at2);

	virtual cpl laplaceExponent(const cpl & x) const;
    private:
	const boost::shared_ptr<const Heston> h;
	const double t1;
	const double t2;
    };

    void heston_try();
}

#endif
