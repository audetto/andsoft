#include <asml/analytics/implied.h>

#include <math.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>

namespace
{
    double intrinsic(double x, double theta)
    {
	if (theta * x > 0.0)
	{
	    return theta * (exp(x * 0.5) - exp(-x * 0.5));
	}
	else
	{
	    return 0.0;
	}
    }

    double b(double x, double sigma, double theta)
    {
	if (sigma == 0.0)
	    return intrinsic(x, theta);

	const double d1 = x / sigma + 0.5 * sigma;
	const double d2 = x / sigma - 0.5 * sigma;
	const double phi1 = gsl_cdf_ugaussian_P(theta * d1);
	const double phi2 = gsl_cdf_ugaussian_P(theta * d2);

	const double price = theta * (exp(x * 0.5) * phi1 - exp(-x * 0.5) * phi2);
	return price;
    }

    double b_to_0(double x, double sigma, double theta)
    {
	const double opt = x * gsl_ran_ugaussian_pdf(x / sigma) * pow(sigma / x, 3);
	const double intr = intrinsic(x, theta);

	return intr + opt;
    }

    double b_to_inf(double x, double sigma, double theta)
    {
	const double tmp1 = exp(0.5 * theta * x);
	const double tmp2 = 0.25 / sigma * gsl_ran_ugaussian_pdf(0.5 * sigma);

	return tmp1 - tmp2;
    }

    double b_c(double x, double theta)
    {
	const double sigma_c = sqrt(2.0 * fabs(x));
	return b(x, sigma_c, theta);
    }

    double sigma_0(double beta, double x, double theta)
    {
	const double bc = b_c(x, theta);
	if (beta < bc)
	{
	    const double intr = intrinsic(x, theta);
	    const double sigma_low = sqrt(2.0 * x * x / (fabs(x) - 4.0 * log((beta - intr) / (bc - intr))));  // eq 3.6
	    return sigma_low;
	}
	else
	{
	    const double phi = gsl_cdf_ugaussian_P(-sqrt(fabs(x) * 0.5));
	    const double expo = exp(theta * x * 0.5);
	    const double ratio = (expo - beta) / (expo - bc);
	    const double sigma_high = -2 * gsl_cdf_ugaussian_Pinv(ratio * phi); // eq 3.7
	    return sigma_high;
	}
    }

    double step(double x, double sigma, double theta, double beta)
    {
	const double bc = b_c(x, theta);
	const double theB = b(x, sigma, theta);
	const double dBdSigma = exp(-0.5 *(x * x / sigma / sigma + sigma * sigma * 0.25)) / sqrt(2.0 * M_PI); // eq 3.10

	if (beta < bc)
	{
	    const double intr = intrinsic(x, theta);
	    const double theStep = log((beta - intr) / (theB - intr)) * log(theB - intr) / log(beta - intr) * (theB - intr) / dBdSigma; // eq 4.10
	    return theStep;
	}
	else
	{
	    const double theStep = (beta - theB) / dBdSigma; // eq 4.10
	    return theStep;
	}
    }

    double impliedFromB(double beta, double x, double theta, double tol)
    {
	if (x == 0.0)
	{
	    const double sigma = -2.0 * gsl_cdf_ugaussian_Pinv(0.5 * (1.0 - beta));
	    return sigma;
	}

	double sigma = sigma_0(beta, x, theta);

	for (int i = 0; i < 20; ++i)
	{
	    const double theStep = step(x, sigma, theta, beta);
	    sigma += theStep;
	    if (fabs(theStep / sigma) < tol)
		break;
	}
	return sigma;
    }

}

namespace ASI
{

    using namespace std;

    double impliedVolatility(double price, double forward, double strike, double time, double theta)
    {
	const double x = log(forward / strike);
	const double beta = price / sqrt(forward * strike);

	const double sigmaSqrtT = impliedFromB(beta, x, theta, 0.000001);
	const double sigma = sigmaSqrtT / sqrt(time);
	
	return sigma;
    }

    vector<double> variousBS(double forward, double strike, double time, double sigma, double theta)
    {
	vector<double> result(3);
	
	const double x = log(forward / strike);
	const double sigmaSqrtT = sigma * sqrt(time);
	const double mult = sqrt(forward * strike);

	result[0] = b(x, sigmaSqrtT, theta) * mult;
	result[1] = b_to_0(x, sigmaSqrtT, theta) * mult;
	result[2] = b_to_inf(x, sigmaSqrtT, theta) * mult;

	return result;
    }

    double blackCallPrice(double logStrike, double sigmaSqrtT)
    {
	const double x = -logStrike;
	const double mult = exp(0.5 * logStrike);
	const double callTheta = 1.0;
	const double price = b(x, sigmaSqrtT, callTheta) * mult;

	return price;
    }
}
