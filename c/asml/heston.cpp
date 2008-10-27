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

	cpl logPhi(const cpl & u, const cpl & v, double t) const;  // v = 0 is the canonical Heston Characteristic function
	cpl logPhiFwd(const cpl & u, double t1, double t2) const;  // characteristic function of S(T2) / S(T1) (forward starting option)

	void getCD(const cpl & u, const cpl & v, double t, cpl & C, cpl & D) const;

	cpl log_fx(const cpl & u, const cpl & offset, double t1, double t2, double k) const;
	double fx_over_iu_0(const cpl & offset, double t1, double t2, double k) const;
	double fx_over_iu(double u, const cpl & offset, double t1, double t2, double k) const;

	double get_C_inf(double t1, double t2) const;
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

    cpl Heston::logPhiFwd(const cpl & u, double t1, double t2) const
    {
	// this the part after the strike
	cpl C2, D2;
	getCD(u, 0.0, t2 - t1, C2, D2);

	// first part, forward starting
	cpl C1, D1;
	const cpl v = D2 / I;
	getCD(0.0, v, t1, C1, D1);

	const cpl result = (C1 + C2) + D1 * variance;

	return result;
    }

    cpl Heston::log_fx(const cpl & u, const cpl & offset, double t1, double t2, double k) const
    {
	return logPhiFwd(u + offset, t1, t2) - I * u * log(k);
    }

    double Heston::fx_over_iu_0(const cpl & offset, double t1, double t2, double k) const
    {
	/*
	  This is done numerically because Mathematica canno tfind a formula for the case t1 > 0.0
	 */
	cpl u_down = -0.0001;
	cpl u_mid  =  0.0000;
	cpl u_up   = +0.0001;

	const cpl phi_down = log_fx(u_down, offset, t1, t2, k);
	const cpl phi_mid  = log_fx(u_mid,  offset, t1, t2, k);
	const cpl phi_up   = log_fx(u_up,   offset, t1, t2, k);
	const cpl J_der    = (phi_up - phi_down) / (u_up - u_down);

	const double value = exp(phi_mid.real()) * cos(phi_mid.imag()) * J_der.imag();
	return value;
    }

    double Heston::fx_over_iu(double u, const cpl & offset, double t1, double t2, double k) const
    {
	if (u == 0.0)
	{
	    return fx_over_iu_0(offset, t1, t2, k);
	}

	const cpl f = exp(log_fx(u, offset, t1, t2, k)) / (I * u);
	double result = f.real();
	return result;
    }

    double Heston::get_C_inf(double t1, double t2) const
    {
	double equivalent_t = t2 - t1;

	/*
	  We want to compute here Re(logPhiFwd(u, t1, t2) / u) as u -> +INF

	  Imagine we fix t2 - t1 and let t1 -> 0.0

	  ((C1 + C2) + D1 * variance / u)

	  C2 / u is easy, done by PJ (on t2 - t1)
	  C1 / u seems to be 0.0

	  D1(D2(u))/u = ~ A / ((exp(-k t1) - 1) u) -> 0.0
	  If t1 == 0.0, ~ A (see PJ)
	  If t1 > 0.0,  ~ 0

	  This means that in the following formula we should set variance to 0.0 if t1 > 0.0 (which is confirmed by experiments)
	  but it does not seem to work very well (gsl's integral takes very long). Using PJ's value for t2-t1 seems to be better.
	 */


	double c_inf = sqrt(1.0 - square(rho)) / alpha * (variance + kappa * theta * equivalent_t);
	return c_inf;
    }

    double normal(double u, double t1, double t2, double k, const Heston & h)
    {
	double y = (h.fx_over_iu(u, -I, t1, t2, k) - k * h.fx_over_iu(u, 0.0, t1, t2, k)) / M_PI;
	return y;
    }

    double pj_func(double x, double t1, double t2, double k, const Heston & h)
    {
	if (x == 0.0)
	    return 0.0;

	double c_inf = h.get_C_inf(t1, t2);
	double u = -log(x) / c_inf;
	double y = normal(u, t1, t2, k, h) / (x * c_inf);
	return y;
    }

    struct gsl_heston
    {
	const double t1;
	const double t2;
	const double k;
	const Heston & h;
	gsl_heston(const double at1, const double at2, const double ak, const Heston & ah) : t1(at1), t2(at2), k(ak), h(ah) {}
    };

    double gsl_wrapper(double x, void * params)
    {
	const gsl_heston * gh = static_cast<const gsl_heston *>(params);
	return pj_func(x, gh->t1, gh->t2, gh->k, gh->h);
	//	return normal(x, gh->t1, gh->t2, gh->k, gh->h);
    }

}

namespace ASI
{
    double hestonCallPrice(const double strike, const double time1, const double time2, const double sigma, const double kappa, const double theta, const double alpha, const double rho, size_t n)
    {
	const Heston heston(sigma, kappa, theta, alpha, rho);

	gsl_heston data(time1, time2, strike, heston);

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
	if (sigmaSqrtT == 0.0)
	    return max(1.0 - strike, 0.0);

	const double d1 = -log(strike) / sigmaSqrtT + 0.5 * sigmaSqrtT;
	const double d2 = -log(strike) / sigmaSqrtT - 0.5 * sigmaSqrtT;
	const double callPrice = gsl_cdf_ugaussian_P(d1) - strike * gsl_cdf_ugaussian_P(d2);
	return callPrice;
    }

    void heston_try()
    {
	Heston h(0.2, 2.0, 0.25, 0.5, -0.5);

	const cpl & offset = -I;
	const double t1 = 1.0;
	const double t2 = t1 + 10.0;

	printf("f(%g)=%g\n", 0.0, h.fx_over_iu(0.0, offset, t1, t2, 0.9));
	printf("f(%g)=%g\n", 0.000001, h.fx_over_iu(0.000001, offset, t1, t2, 0.9));
	printf("f(%g)=%g\n", 0.000002, h.fx_over_iu(0.000002, offset, t1, t2, 0.9));
	printf("f(%g)=%g\n", 0.000003, h.fx_over_iu(0.000003, offset, t1, t2, 0.9));

	const double call = hestonCallPrice(0.9, t1, t2, 0.2, 2.0, 0.25, 0.5, -0.4, 200);
	printf("Call = %g\n\n", call);

	printf("C INF=%g\n", h.get_C_inf(t1, t2));
    }
}
