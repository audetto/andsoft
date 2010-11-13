/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "ASIMaths_impl.h"

#include <asml/linearalgebra/projection.h>
#include <asml/linearalgebra/linear.h>
#include <asml/distribution/heston.h>
#include <asml/analytics/implied.h>
#include <asml/analytics/numerics.h>
#include <asml/distribution/fourierpricing.h>
#include <asml/linearalgebra/fastexp.h>

#include <boost/regex.hpp> 

#include "ooutils.h"
#include "ConversionOut.h"
#include "ConversionIn.h"

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::reflection;
using namespace ::com::sun::star::uno;
using namespace ASI;
using namespace std;

namespace _ASIMaths_impl_
{
    
    OUString SAL_CALL ASIMaths_impl::getType( const Any& data ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        OUString typeName = data.getValueTypeName();

        return typeName;

        WRAP_END;
    }

    Sequence< Sequence< OUString > > SAL_CALL ASIMaths_impl::getExceptions( const Any & ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        vector<string> e;
        ASI::getExceptions(e);

        return ooDirectConvert<Sequence< Sequence< OUString > > >(e);

        WRAP_END;
    }

    OUString SAL_CALL ASIMaths_impl::saveValue( const OUString & name, double value ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        string str;
        ooConvertIn(name, str);
        
        boost::shared_ptr<const double> v(new double(value));
        
        return ooDirectConvert<OUString>(std::make_pair(str, v));

        WRAP_END;
    }

    double SAL_CALL ASIMaths_impl::getValue( const OUString & name ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        boost::shared_ptr<const double> v;
        ooConvertIn(name, v);

        return *v;

        WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::projection( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException)
    {
        WRAP_BEGIN;
        
        VectorPtr  xVect;
        ooConvertIn(x, xVect);
        
        MatrixPtr aMat;
        ooConvertIn(a, aMat);
        
        VectorPtr bVect;
        ooConvertIn(b, bVect);
        
        projectOnSubspace(xVect.get(), aMat.get(), bVect.get());
        
        return ooDirectConvert<Sequence<Sequence<double> >, VectorPtr >(xVect);
        
        WRAP_END;
    }
    
    Sequence<Sequence<double> > ASIMaths_impl::dykstra( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException)
    {
        WRAP_BEGIN;
        
        VectorPtr  xVect;
        ooConvertIn(x, xVect);

        MatrixPtr aMat;
        ooConvertIn(a, aMat);
        
        VectorPtr bVect;
        ooConvertIn(b, bVect);
        
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
        ooConvertIn(a, aMat);
        
        VectorPtr bVect;
        ooConvertIn(b, bVect);
        
        VectorPtr xVect;
        
        ASI::svdSolve(aMat, bVect, xVect);
        
        return ooDirectConvert<Sequence<Sequence<double> >, VectorPtr >(xVect);
        
        WRAP_END;
    }
    
    Sequence<Sequence<double> > ASIMaths_impl::fastExp( double t, const Sequence<Sequence<double> > & a, sal_Int32 method, double theta) throw (RuntimeException)
    {
        WRAP_BEGIN;
        
        MatrixPtr aMat;
        ooConvertIn(a, aMat);
        
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
        ooConvertIn(xa, xVect);
        
        std::vector<double> yVect;
        ooConvertIn(ya, yVect);
        
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
        ooConvertIn(data, dataVect);
        ASI::FFT_Real(dataVect, true);
        return ooDirectConvert<Sequence<Sequence<double> >, std::vector<double> >(dataVect);
        
        WRAP_END;
    }
    
    Sequence<Sequence<OUString> > ASIMaths_impl::fftUnpack( const Sequence<Sequence<double> > & data) throw (RuntimeException)
    {
        WRAP_BEGIN;
        
        std::vector<double> dataVect;
        ooConvertIn(data, dataVect);
        std::vector<cpl> result;
        ASI::FFT_Unpack(dataVect, result);
        return ooDirectConvert<Sequence<Sequence<OUString> >, std::vector<cpl> >(result);
        
        WRAP_END;
    }
    
    Sequence<Sequence<double> > ASIMaths_impl::fftPack( const Sequence<Sequence<OUString> > & data) throw (RuntimeException)
    {
        WRAP_BEGIN;
        
        std::vector<cpl> dataVect;
        ooConvertIn(data, dataVect);
        std::vector<double> result;
        ASI::FFT_Pack(dataVect, result);
        return ooDirectConvert<Sequence<Sequence<double> >, std::vector<double> >(result);
        
        WRAP_END;
    }
    
    Sequence<Sequence<double> > ASIMaths_impl::ifft( const Sequence<Sequence<double> > & data) throw (RuntimeException)
    {
        WRAP_BEGIN;
        
        std::vector<double> dataVect;
        ooConvertIn(data, dataVect);
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

    Sequence<Sequence<OUString > > SAL_CALL ASIMaths_impl::regExp( const OUString& ooRegexp, const OUString& ooText ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        std::string regexp;
        ooConvertIn(ooRegexp, regexp);

        std::string text;
        ooConvertIn(ooText, text);

        boost::regex expression(regexp); 

        vector<vector<string> > result;

        boost::smatch what;
        boost::regex_match(text, what, expression);

        const size_t numberOfResults = what.size();

        result.resize(numberOfResults);
        for (size_t i = 0; i < numberOfResults; ++i)
        {
            result[i].resize(1);
            if (what[i].matched)
                result[i][0] = what[i];
        }
        
        return ooDirectConvert<Sequence<Sequence<OUString > > >(result);

        WRAP_END;
   } 

}
