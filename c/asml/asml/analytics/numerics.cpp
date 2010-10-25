#include <asml/analytics/numerics.h>
#include <asml/utils/utils.h>

#include <math.h>
#include <limits>
#include <stdio.h>

#include <gsl/gsl_interp.h>

namespace ASI
{
    using namespace std;

    double decayIntegral(double x)
    {
	static const double lim = 24.0 * pow(std::numeric_limits<double>::epsilon(), 1.0 / 3.0);

	if (fabs(x) < lim)
	    return 1.0 - 0.5 * x * (1.0 - x / 3.0) ;
	else
	    return (1.0 - exp(-x)) / x;
    }

    double decayIntegral2(double x)
    {
	static const double lim = 120.0 * pow(std::numeric_limits<double>::epsilon(), 1.0 / 3.0);

	if (fabs(x) < lim)
	    return -0.5 + x / 6.0 * (1.0 - x / 4.0);
	else
	    return (decayIntegral(x) - 1.0) / x;
    }


    double finiteDifference(const vector<double> & xa, const vector<double> & ya, double x, size_t order)
    {
	const size_t dim = xa.size();
	if (ya.size() != dim)
	    error("Invalid size");

	gsl_interp * interp = gsl_interp_alloc (gsl_interp_polynomial, dim);

	gsl_interp_init (interp, &xa[0], &ya[0], dim);

	double result;

	switch (order)
	{
	case 0:
	    result = gsl_interp_eval(interp, &xa[0], &ya[0], x, NULL);
	    break;
	case 1:
	    result = gsl_interp_eval_deriv(interp, &xa[0], &ya[0], x, NULL);
	    break;
	case 2:
	    result = gsl_interp_eval_deriv2(interp, &xa[0], &ya[0], x, NULL);
	    break;
	default:
	    error("Invalid order");
	}

	gsl_interp_free (interp);

	return result;
    }


    void numerics_try()
    {
	const double eps = std::numeric_limits<double>::epsilon() * 1;

	for (int i = 0; i < 200; ++i)
	{
	    const double x = eps * i;
	    const double y1 = decayIntegral2(x);

	    printf("%d: %+.20f, %+.20f\n", i, x, y1);
	}
    }
}
