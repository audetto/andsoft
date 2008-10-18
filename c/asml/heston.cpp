#include <asi/heston.h>
#include <asi/numerics.h>

#include <complex>
#include <math.h>

#include <gsl/gsl_integration.h>
#include <gsl/gsl_cdf.h>

using namespace std;
using namespace ASI;

namespace
{

    typedef complex<double> cpl;

    template<typename T>
    T square(const T & x)
    {
	return x * x;
    }

    const cpl I(0.0, 1.0);

    class Heston
    {
    private:
	const double variance;    // V0
	const double kappa;       // mean reversion speed
	const double theta;       // long term variance
	const double alpha;       // vol of variance
	const double rho;
    public:
	Heston(const double asigma, const double akappa, const double atheta, const double aalpha, const double arho) : 
	    variance(square(asigma)), kappa(akappa), theta(atheta), alpha(aalpha), rho(arho) {}

	cpl logphi(const cpl & u, const cpl & v, double t) const;
	double f1(double u, double t, double k) const;
	double f2(double u, double t, double k) const;

	double get_C_inf(double t) const;
   };

    cpl Heston::logphi(const cpl & u, const cpl & v, double t) const
    {

	const cpl rliu = rho * alpha * u * I;
	const cpl d  = sqrt(square(rliu - kappa) + square(alpha) * u * (I + u));
	const cpl g  = (kappa - rliu - d                        ) / (kappa - rliu + d                        );
	const cpl gg = (kappa - rliu - d - v * I * square(alpha)) / (kappa - rliu + d - v * I * square(alpha));

	const cpl C = theta * kappa / square(alpha) * ( (kappa - rliu - d) * t - 2.0 * log((1.0 - gg * exp(-d * t)) / (1.0 - gg)) );
	const cpl D = square(1.0 / alpha) * (kappa - rliu + d) * (g - gg * exp(- d * t)) / (1.0 - gg * exp(-d * t));

	const cpl result = C + D * variance;

	return result;
    }

    double Heston::f1(double u, double t, double k) const
    {
	if (u == 0.0)
	{
	    const double arg = t * (kappa - rho * alpha);
	    const double C = - 0.5 * theta * kappa * square(t) * decayIntegral2(arg);
	    const double D = 0.5 * t * decayIntegral(arg);

	    const double f1_0 = -log(k) + C + D * variance;
	    return f1_0;
	}

	const cpl logp = logphi(u - I, 0.0, t);
	const cpl f = exp(- I * u * log(k) + logp) / (I * u);
	double result = f.real();
	return result;
    }

    double Heston::f2(double u, double t, double k) const
    {
	if (u == 0.0)
	{
	    const double arg = t * kappa;
	    const double C = 0.5 * theta * kappa * square(t) * decayIntegral2(arg);
	    const double D = -0.5 * t * decayIntegral(arg);

	    const double f2_0 = -log(k) + C + D * variance;
	    return f2_0;
	}	

	const cpl logp = logphi(u, 0.0, t);
	const cpl f = exp(- I * u * log(k) + logp) / (I * u);
	double result = f.real();
	return result;
    }

    double Heston::get_C_inf(double t) const
    {
	double c_inf = sqrt(1.0 - square(rho)) / alpha * (variance + kappa * theta * t);
	return c_inf;
    }

    double normal(double u, double t, double k, const Heston & h)
    {
	double y = (h.f1(u, t, k) - k * h.f2(u, t, k)) / M_PI;
	return y;
    }

    double pj_func(double x, double t, double k, const Heston & h)
    {
	if (x == 0.0)
	    return 0.0;

	double c_inf = h.get_C_inf(t);
	double u = -log(x) / c_inf;
	double y = (h.f1(u, t, k) - k * h.f2(u, t, k)) / (x * M_PI * c_inf);
	return y;
    }

    struct gsl_heston
    {
	const double t;
	const double k;
	const Heston & h;
	gsl_heston(const double at, const double ak, const Heston & ah) : t(at), k(ak), h(ah) {}
    };

    double gsl_wrapper(double x, void * params)
    {
	const gsl_heston * gh = static_cast<const gsl_heston *>(params);
	return pj_func(x, gh->t, gh->k, gh->h);
	//	return normal(x, gh->t, gh->k, gh->h);
    }

}

namespace ASI
{
    double hestonCallPrice(const double strike, const double time, const double sigma, const double kappa, const double theta, const double alpha, const double rho, size_t n)
    {
	const Heston heston(sigma, kappa, theta, alpha, rho);

	gsl_heston data(time, strike, heston);

	gsl_function f;
	f.function = &gsl_wrapper;
	f.params = &data;

	gsl_integration_workspace * ws = gsl_integration_workspace_alloc (n);

	double epsabs = 0.00000001;
	double epsrel = 0.00000001;
	size_t limit = n - 10;
	int key = 3;

	double price;
	double abserr;

	int res = gsl_integration_qag (&f, 0.0, 1.0, epsabs, epsrel, limit, key, ws, &price, &abserr);
	//	int res = gsl_integration_qag (&f, 0.0, 100.0, epsabs, epsrel, limit, key, ws, &price, &abserr);

	gsl_integration_workspace_free (ws);

	return 0.5 * (1.0 - strike) + price;
    }

    double blackCallPrice(const double strike, const double time, const double sigma)
    {
	const double sigmaSqrtT = sigma * sqrt(time);
	const double d1 = -log(strike) / sigmaSqrtT + 0.5 * sigmaSqrtT;
	const double d2 = -log(strike) / sigmaSqrtT - 0.5 * sigmaSqrtT;
	const double callPrice = gsl_cdf_ugaussian_P(d1) - strike * gsl_cdf_ugaussian_P(d2);
	return callPrice;
    }

    void heston_try()
    {
	Heston h(0.2, 2.0, 0.25, 0.5, -0.5);

	printf("f1(%g)=%g\n", 0.0, h.f1(0.0, 1.5, 0.9));
	printf("f1(%g)=%g\n", 0.000001, h.f1(0.000001, 1.5, 0.9));
	printf("f1(%g)=%g\n", 0.000002, h.f1(0.000002, 1.5, 0.9));
	printf("f1(%g)=%g\n", 0.000003, h.f1(0.000003, 1.5, 0.9));
	printf("f2(%g)=%g\n", 0.0, h.f2(0.0, 1.5, 0.9));
	printf("f2(%g)=%g\n", 0.000001, h.f2(0.000001, 1.5, 0.9));
	printf("f2(%g)=%g\n", 0.000002, h.f2(0.000002, 1.5, 0.9));
	printf("f2(%g)=%g\n", 0.000003, h.f2(0.000003, 1.5, 0.9));
	
    }
}
