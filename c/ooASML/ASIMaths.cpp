#include "ASIMaths.h"

#include <asml/projection.h>
#include <asml/linear.h>
#include <asml/heston.h>
#include <asml/implied.h>
#include <asml/numerics.h>
#include <asml/fourierpricing.h>
#include <asml/fastexp.h>

#include "ooutils.h"
#include "conversion.h"

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::reflection;
using namespace ::com::sun::star::uno;
using namespace ASI;

namespace _ASIMaths_impl_
{

    //expo(mean)
    double ASIMaths_impl::expo( double m ) throw (RuntimeException)
    {
	return -m * log( (double)(1+(unsigned int)rand()) / (2+(unsigned int)RAND_MAX) );
    }    

    Sequence<Sequence<double> > ASIMaths_impl::projection( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException)
    {
	WRAP_BEGIN;

	VectorPtr  xVect;
	ooConvert(x, xVect);

	MatrixPtr aMat;
	ooConvert(a, aMat);

	VectorPtr bVect;
	ooConvert(b, bVect);

	projectOnSubspace(xVect.get(), aMat.get(), bVect.get());
	
	return ooDirectConvert<Sequence<Sequence<double> >, VectorPtr >(xVect);

	WRAP_END;
    }
    
    Sequence<Sequence<double> > ASIMaths_impl::dykstra( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException)
    {
	WRAP_BEGIN;

	VectorPtr  xVect;
	ooConvert(x, xVect);

	MatrixPtr aMat;
	ooConvert(a, aMat);

	VectorPtr bVect;
	ooConvert(b, bVect);

	const size_t iter = 100;
	const double tol = 1.0e-10;

	dykstraProjection(xVect.get(), aMat.get(), bVect.get(), iter, tol);
	
	return ooDirectConvert<Sequence<Sequence<double> >, VectorPtr >(xVect);

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::svdSolve( const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException)
    {
	WRAP_BEGIN;

	MatrixPtr aMat;
	ooConvert(a, aMat);

	VectorPtr bVect;
	ooConvert(b, bVect);

	VectorPtr xVect;

	ASI::svdSolve(aMat, bVect, xVect);
	
	return ooDirectConvert<Sequence<Sequence<double> >, VectorPtr >(xVect);

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fastExp( double t, const Sequence<Sequence<double> > & a, sal_Int32 method, double theta) throw (RuntimeException)
    {
	WRAP_BEGIN;

	MatrixPtr aMat;
	ooConvert(a, aMat);

	MatrixPtr exptA;

	switch (method)
	{
	case 0:
	    exptA = expViaEigenvalues(t, aMat);
	    break;
	case 1:
	    exptA = expViaGSL(t, aMat);
	    break;
	default:
	    exptA = expViaTheta(t, aMat, method, theta);
	    break;
	}

	return ooDirectConvert<Sequence<Sequence<double> >, MatrixPtr >(exptA);

	WRAP_END;
    }

    double ASIMaths_impl::finiteDifference( const Sequence<Sequence<double> > & xa, const Sequence<Sequence<double> > & ya, double x, sal_Int32 order) throw (RuntimeException)
    {
 	WRAP_BEGIN;

	std::vector<double> xVect;
	ooConvert(xa, xVect);

	std::vector<double> yVect;
	ooConvert(ya, yVect);
	
	const double result = ASI::finiteDifference(xVect, yVect, x, order);

	return result;

	WRAP_END;
    }

    double ASIMaths_impl::heston( double strike, double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 points) throw (RuntimeException)
    {
	WRAP_BEGIN;

	const double price = hestonCallPrice(strike, time1, time2, sigma, kappa, theta, alpha, rho, points);
	return price;

	WRAP_END;
    }

    double ASIMaths_impl::black( double strike, double time, double sigma) throw (RuntimeException)
    {
	WRAP_BEGIN;

	const double sigmaSqrtT = sigma * sqrt(time);
	const double logStrike  = log(strike);
	const double price = blackCallPrice(logStrike, sigmaSqrtT);
	return price;

	WRAP_END;
    }

    double ASIMaths_impl::impliedVolatility( double price, double strike, double time ) throw (RuntimeException)
    {
 	WRAP_BEGIN;

	const double sigma = ASI::impliedVolatility(price, 1.0, strike, time, 1.0);
	return sigma;

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::variousBS( double strike, double time, double sigma) throw (RuntimeException)
    {
	WRAP_BEGIN;

	const std::vector<double> res = ASI::variousBS(1.0, strike, time, sigma, 1.0);
	return ooDirectConvert<Sequence<Sequence<double> >, std::vector<double> >(res);

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fft( const Sequence<Sequence<double> > & data) throw (RuntimeException)
    {
	WRAP_BEGIN;

	std::vector<double> dataVect;
	ooConvert(data, dataVect);
	ASI::FFT_Real(dataVect, true);
	return ooDirectConvert<Sequence<Sequence<double> >, std::vector<double> >(dataVect);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fft_unpack( const Sequence<Sequence<double> > & data) throw (RuntimeException)
    {
	WRAP_BEGIN;

	std::vector<double> dataVect;
	ooConvert(data, dataVect);
	std::vector<cpl> result;
	ASI::FFT_Unpack(dataVect, result);
	return ooDirectConvert<Sequence<Sequence<double> >, std::vector<cpl> >(result);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fft_pack( const Sequence<Sequence<double> > & data) throw (RuntimeException)
    {
	WRAP_BEGIN;

	std::vector<cpl> dataVect;
	ooConvert(data, dataVect);
	std::vector<double> result;
	ASI::FFT_Pack(dataVect, result);
	return ooDirectConvert<Sequence<Sequence<double> >, std::vector<double> >(result);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::ifft( const Sequence<Sequence<double> > & data) throw (RuntimeException)
    {
	WRAP_BEGIN;

	std::vector<double> dataVect;
	ooConvert(data, dataVect);
	ASI::FFT_Real(dataVect, false);
	return ooDirectConvert<Sequence<Sequence<double> >, std::vector<double> >(dataVect);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::hestonFFT( double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 N, double stdDev) throw (RuntimeException)
    {
	WRAP_BEGIN;

	std::vector<double> strikes, prices;
	ASI::HestonDistribution heston(sigma, kappa, theta, alpha, rho, time1, time2);
	ASI::priceViaFFT(N, stdDev, heston, strikes, prices);

	Sequence<Sequence<double> > result;

	appendStdVectorToOOArgument(result, strikes);
	appendStdVectorToOOArgument(result, prices);

	return result;

	WRAP_END;
    }

}
