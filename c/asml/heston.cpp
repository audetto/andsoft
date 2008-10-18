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

	cpl logPhi(const cpl & u, const cpl & v, double t) const;

	void getCD(const cpl & u, const cpl & v, double t, cpl & C, cpl & D) const;

	cpl log_fx(const cpl & u, const cpl & offset, double t, double k) const;
	double fx_over_iu_0(const cpl & offset, double t, double k) const;
	double fx_over_iu(double u, const cpl & offset, double t, double k) const;

	double get_C_inf(double t) const;
    };

    void Heston::getCD(const cpl & u, const cpl & v, double t, cpl & C, cpl & D) const
    {

	const cpl rliu = rho * alpha * u * I;
	const cpl d  = sqrt(square(rliu - kappa) + square(alpha) * u * (I + u));
	const cpl g  = (kappa - rliu - d                        ) / (kappa - rliu + d                        );
	const cpl gg = (kappa - rliu - d - v * I * square(alpha)) / (kappa - rliu + d - v * I * square(alpha));

	C = theta * kappa / square(alpha) * ( (kappa - rliu - d) * t - 2.0 * log((1.0 - gg * exp(-d * t)) / (1.0 - gg)) );
	D = square(1.0 / alpha) * (kappa - rliu + d) * (g - gg * exp(- d * t)) / (1.0 - gg * exp(-d * t));
    }

    cpl Heston::logPhi(const cpl & u, const cpl & v, double t) const
    {
	cpl C, D;
	getCD(u, v, t, C, D);
	const cpl result = C + D * variance;

	return result;
    }

    cpl Heston::log_fx(const cpl & u, const cpl & offset, double t, double k) const
    {
	return logPhi(u + offset, 0.0, t) - I * u * log(k);
    }

    double Heston::fx_over_iu_0(const cpl & offset, double t, double k) const
    {
	cpl u_down = -0.001;
	cpl u_mid  = 0.0;
	cpl u_up   = +0.001;

	const cpl phi_down = log_fx(u_down, offset, t, k);
	const cpl phi_mid  = log_fx(u_mid,  offset, t, k);
	const cpl phi_up   = log_fx(u_up,   offset, t, k);
	const cpl J_der    = (phi_up - phi_down) / (u_up - u_down);

	const double value = exp(phi_mid.real()) * cos(phi_mid.imag()) * J_der.imag();
	return value;
    }

    double Heston::fx_over_iu(double u, const cpl & offset, double t, double k) const
    {
	if (u == 0.0)
	{
	    return fx_over_iu_0(offset, t, k);
	}

	const cpl f = exp(log_fx(u, offset, t, k)) / (I * u);
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
	double y = (h.fx_over_iu(u, -I, t, k) - k * h.fx_over_iu(u, 0.0, t, k)) / M_PI;
	return y;
    }

    double pj_func(double x, double t, double k, const Heston & h)
    {
	if (x == 0.0)
	    return 0.0;

	double c_inf = h.get_C_inf(t);
	double u = -log(x) / c_inf;
	double y = normal(u, t, k, h) / (x * c_inf);
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

	const cpl & offset = -I;

	printf("f(%g)=%g\n", 0.0, h.fx_over_iu(0.0, offset, 1.5, 0.9));
	printf("f(%g)=%g\n", 0.000001, h.fx_over_iu(0.000001, offset, 1.5, 0.9));
	printf("f(%g)=%g\n", 0.000002, h.fx_over_iu(0.000002, offset, 1.5, 0.9));
	printf("f(%g)=%g\n", 0.000003, h.fx_over_iu(0.000003, offset, 1.5, 0.9));
	
    }
}
