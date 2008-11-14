#include <cppuhelper/implbase4.hxx> //4-parameter template will be used
#include <cppuhelper/factory.hxx>
#include <cppuhelper/implementationentry.hxx>

#include <com/sun/star/sheet/XAddIn.hpp>
#include <com/sun/star/lang/XServiceName.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/IllegalArgumentException.hpp>
#include <com/sun/star/reflection/XIdlReflection.hpp>
#include <org/asi/XMaths.hpp>

#include <stdio.h>
#include <cstdlib>
#include <cmath>

#include <asi/projection.h>
#include <asi/linear.h>
#include <asi/heston.h>
#include <asi/implied.h>
#include <asi/numerics.h>
#include <asi/fourierpricing.h>
#include <asi/fastexp.h>

#include "ooutils.h"
#include "conversion.h"

#define FUNK  /* printf("%s\n", __func__) */

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::reflection;
using namespace ::com::sun::star::uno;
using namespace ASI;

namespace _ASIMaths_impl_
{
    class ASIMaths_impl : public ::cppu::WeakImplHelper4< org::asi::XMaths,
						     sheet::XAddIn, lang::XServiceName, lang::XServiceInfo > //4-parameter template
    {
	//Locale
	lang::Locale locale;
	
	
    public:

	static Reference< XIdlClass > ourIDLClass;

	//XMaths
	double SAL_CALL expo( double m ) throw (RuntimeException); //expo(mean)
        Sequence<Sequence<double> > SAL_CALL projection( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException, lang::IllegalArgumentException);
        Sequence<Sequence<double> > SAL_CALL dykstra( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException, lang::IllegalArgumentException);
	Sequence<Sequence<double> > SAL_CALL svdSolve( const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException, lang::IllegalArgumentException);

	Sequence<Sequence<double> > SAL_CALL fastExp( double t, const Sequence<Sequence<double> > & a, sal_Int32 method) throw (RuntimeException, lang::IllegalArgumentException);

	double SAL_CALL finiteDifference( const Sequence<Sequence<double> > & xa, const Sequence<Sequence<double> > & ya, double x, sal_Int32 order) throw (RuntimeException, lang::IllegalArgumentException);
	
	double SAL_CALL heston( double strike, double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 points) throw (RuntimeException, lang::IllegalArgumentException);

	double SAL_CALL black( double strike, double time, double sigma) throw (RuntimeException, lang::IllegalArgumentException);

	double SAL_CALL impliedVolatility( double price, double strike, double time ) throw (RuntimeException, lang::IllegalArgumentException);

	Sequence<Sequence<double> > SAL_CALL variousBS( double time, double strike, double sigma) throw (RuntimeException, lang::IllegalArgumentException);

	Sequence<Sequence<double> > SAL_CALL fft( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException);
	Sequence<Sequence<double> > SAL_CALL ifft( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException);
	Sequence<Sequence<double> > SAL_CALL fft_unpack( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException);
	Sequence<Sequence<double> > SAL_CALL fft_pack( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException);

	Sequence<Sequence<double> > SAL_CALL hestonFFT( double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 N, double stdDev) throw (RuntimeException, lang::IllegalArgumentException);

	//XAddIn
	OUString SAL_CALL getProgrammaticFuntionName( const OUString& aDisplayName ) throw (RuntimeException);
	OUString SAL_CALL getDisplayFunctionName( const OUString& aProgrammaticName ) throw (RuntimeException);
	OUString SAL_CALL getFunctionDescription( const OUString& aProgrammaticName ) throw (RuntimeException);
	OUString SAL_CALL getDisplayArgumentName( const OUString& aProgrammaticName, ::sal_Int32 nArgument ) throw (RuntimeException);
	OUString SAL_CALL getArgumentDescription( const OUString& aProgrammaticName, ::sal_Int32 nArgument ) throw (RuntimeException);
	OUString SAL_CALL getProgrammaticCategoryName( const OUString& aProgrammaticName ) throw (RuntimeException);
	OUString SAL_CALL getDisplayCategoryName( const OUString& aProgrammaticName ) throw (RuntimeException);
	
	//XServiceName
	OUString SAL_CALL getServiceName(  ) throw (RuntimeException);
	
	//XServiceInfo
	OUString SAL_CALL getImplementationName(  ) throw (RuntimeException);
	::sal_Bool SAL_CALL supportsService( const OUString& ServiceName ) throw (RuntimeException);
	Sequence< OUString > SAL_CALL getSupportedServiceNames(  ) throw (RuntimeException);
	
	//XLocalizable
	void SAL_CALL setLocale( const lang::Locale& eLocale ) throw (RuntimeException);
	lang::Locale SAL_CALL getLocale(  ) throw (RuntimeException);
    };

    Reference< XIdlClass > ASIMaths_impl::ourIDLClass;
    
    //XRNG
    //expo(mean)
    double ASIMaths_impl::expo( double m ) throw (RuntimeException)
    {
	return -m * log( (double)(1+(unsigned int)rand()) / (2+(unsigned int)RAND_MAX) );
    }    

    Sequence<Sequence<double> > ASIMaths_impl::projection( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	VectorPtr  xVect = vectorFromOOArgument(x);
	CMatrixPtr aMat  = matrixFromOOArgument(a);
	CVectorPtr bVect = vectorFromOOArgument(b);

	projectOnSubspace(xVect.get(), aMat.get(), bVect.get());
	
	return vectorToOOArgument(xVect);

	WRAP_END;
    }
    
    Sequence<Sequence<double> > ASIMaths_impl::dykstra( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	VectorPtr  xVect = vectorFromOOArgument(x);
	CMatrixPtr aMat  = matrixFromOOArgument(a);
	CVectorPtr bVect = vectorFromOOArgument(b);

	const size_t iter = 100;
	const double tol = 1.0e-10;

	dykstraProjection(xVect.get(), aMat.get(), bVect.get(), iter, tol);
	
	return vectorToOOArgument(xVect);

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::svdSolve( const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	MatrixPtr aMat  = matrixFromOOArgument(a);
	CVectorPtr bVect = vectorFromOOArgument(b);
	VectorPtr xVect;

	ASI::svdSolve(aMat, bVect, xVect);
	
	return vectorToOOArgument(xVect);

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fastExp( double t, const Sequence<Sequence<double> > & a, sal_Int32 method) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	MatrixPtr aMat = matrixFromOOArgument(a);
	MatrixPtr exptA;

	switch (method)
	{
	case 1:
	    exptA = expViaEigenvalues(t, aMat);
	    break;
	case 2:
	    exptA = expViaGSL(t, aMat);
	    break;
	case 3:
	    exptA = expViaExplicit(t, aMat, 10);
	    break;
	default:
	    error("Invalid method");
	}

	return matrixToOOArgument(exptA);

	WRAP_END;
    }

    double ASIMaths_impl::finiteDifference( const Sequence<Sequence<double> > & xa, const Sequence<Sequence<double> > & ya, double x, sal_Int32 order) throw (RuntimeException, lang::IllegalArgumentException)
    {
 	WRAP_BEGIN;

	const std::vector<double> xVect = stdVectorFromOOArgument(xa);
	const std::vector<double> yVect = stdVectorFromOOArgument(ya);
	
	const double result = ASI::finiteDifference(xVect, yVect, x, order);

	return result;

	WRAP_END;
    }

    double ASIMaths_impl::heston( double strike, double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 points) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	const double price = hestonCallPrice(strike, time1, time2, sigma, kappa, theta, alpha, rho, points);
	return price;

	WRAP_END;
    }

    double ASIMaths_impl::black( double strike, double time, double sigma) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	const double sigmaSqrtT = sigma * sqrt(time);
	const double logStrike  = log(strike);
	const double price = blackCallPrice(logStrike, sigmaSqrtT);
	return price;

	WRAP_END;
    }

    double ASIMaths_impl::impliedVolatility( double price, double strike, double time ) throw (RuntimeException, lang::IllegalArgumentException)
    {
 	WRAP_BEGIN;

	const double sigma = ASI::impliedVolatility(price, 1.0, strike, time, 1.0);
	return sigma;

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::variousBS( double strike, double time, double sigma) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	const std::vector<double> res = ASI::variousBS(1.0, strike, time, sigma, 1.0);
	return stdVectorToOOArgument(res);

	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fft( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	std::vector<double> dataVect = stdVectorFromOOArgument(data);
	ASI::FFT_Real(dataVect, true);
	return stdVectorToOOArgument(dataVect);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fft_unpack( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	const std::vector<double> dataVect = stdVectorFromOOArgument(data);
	std::vector<cpl> result;
	ASI::FFT_Unpack(dataVect, result);
	return stdVectorComplexToOOArgument(result);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::fft_pack( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	const std::vector<cpl> dataVect = stdVectorcomplexFromOOArgument(data);
	std::vector<double> result;
	ASI::FFT_Pack(dataVect, result);
	return stdVectorToOOArgument(result);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::ifft( const Sequence<Sequence<double> > & data) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	std::vector<double> dataVect = stdVectorFromOOArgument(data);
	ASI::FFT_Real(dataVect, false);
	return stdVectorToOOArgument(dataVect);
	
	WRAP_END;
    }

    Sequence<Sequence<double> > ASIMaths_impl::hestonFFT( double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 N, double stdDev) throw (RuntimeException, lang::IllegalArgumentException)
    {
	WRAP_BEGIN;

	std::vector<double> strikes, prices;
	ASI::hestonViaFFT(time1, time2, sigma, kappa, theta, alpha, rho, N, stdDev, strikes, prices);
	Sequence<Sequence<double> > result;

	appendStdVectorToOOArgument(result, strikes);
	appendStdVectorToOOArgument(result, prices);

	return result;

	WRAP_END;
    }

#define _serviceName_ "org.asi.Service"
    
    static const sal_Char *_serviceName = _serviceName_;

//XAddIn
    OUString ASIMaths_impl::getProgrammaticFuntionName( const OUString& aDisplayName ) throw (RuntimeException)
    {
	FUNK;
	return aDisplayName.toAsciiLowerCase();
    }
    
    OUString ASIMaths_impl::getDisplayFunctionName( const OUString& aProgrammaticName ) throw (RuntimeException)
    {
	FUNK;
	return aProgrammaticName.toAsciiUpperCase();
    }
    
    OUString ASIMaths_impl::getFunctionDescription( const OUString& aProgrammaticName ) throw (RuntimeException)
    {
	FUNK;

	if (ourIDLClass.is())
	{
	    Reference< XIdlMethod > aMethod = ourIDLClass->getMethod(aProgrammaticName);
	    if (aMethod.is())
	    {
		Reference< XIdlClass > returnType = aMethod->getReturnType();
		return returnType->getName();
	    }
	}

	return aProgrammaticName;
    }
    
    OUString ASIMaths_impl::getDisplayArgumentName( const OUString& aProgrammaticName, ::sal_Int32 nArgument ) throw (RuntimeException)
    {
	FUNK;

	if (ourIDLClass.is())
	{
	    Reference< XIdlMethod > aMethod = ourIDLClass->getMethod(aProgrammaticName);
	    if (aMethod.is())
	    {
		Sequence< ParamInfo > params = aMethod->getParameterInfos();
		return params[nArgument].aName;
	    }
	}

	OUString out = aProgrammaticName + OUString(RTL_CONSTASCII_USTRINGPARAM(": ")) + OUString::valueOf(nArgument);
	return out;
    }
    
    OUString ASIMaths_impl::getArgumentDescription( const OUString& aProgrammaticName, ::sal_Int32 nArgument ) throw (RuntimeException)
    {
	FUNK;
	if (ourIDLClass.is())
	{
	    Reference< XIdlMethod > aMethod = ourIDLClass->getMethod(aProgrammaticName);
	    if (aMethod.is())
	    {
		Sequence< Reference < XIdlClass > > params = aMethod->getParameterTypes();
		return params[nArgument]->getName();
	    }
	}

	return getDisplayArgumentName(aProgrammaticName, nArgument);
    }
    
    OUString ASIMaths_impl::getProgrammaticCategoryName( const OUString& aProgrammaticName ) throw (RuntimeException)
    {
	FUNK;
	// This is not supported at the moment!
	return OUString(RTL_CONSTASCII_USTRINGPARAM("ASI"));
    }
    
    OUString ASIMaths_impl::getDisplayCategoryName( const OUString& aProgrammaticName ) throw (RuntimeException)
    {
	FUNK;
	return getProgrammaticCategoryName( aProgrammaticName );
    }
    
    //XServiceName
    OUString ASIMaths_impl::getServiceName(  ) throw (RuntimeException)
    {
	FUNK;
	return OUString::createFromAscii(_serviceName);
    }
    
    //XServiceInfo
    static OUString getImplementationName_ASIMaths_impl() throw (RuntimeException)
    {
	FUNK;
	return OUString(RTL_CONSTASCII_USTRINGPARAM("org.asi.ASIMaths_impl.Service"));
    }
    
    OUString ASIMaths_impl::getImplementationName() throw (RuntimeException)
    {
	FUNK;
	return getImplementationName_ASIMaths_impl();
    }
    
    ::sal_Bool ASIMaths_impl::supportsService( OUString const & serviceName ) throw (RuntimeException)
    {
	FUNK;
	if (serviceName.equalsAscii( _serviceName ))
	    return true;
	if (serviceName.equalsAsciiL( RTL_CONSTASCII_STRINGPARAM("com.sun.star.sheet.AddIn") ))
	    return true;
	else
	    return false;
    }
    
    static Sequence< OUString > getSupportedServiceNames_ASIMaths_impl() throw (RuntimeException)
    {
	FUNK;
	Sequence< OUString > names(2);
	names[0] = OUString::createFromAscii(_serviceName);
	names[1] = OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.sheet.AddIn"));
	
	return names;
    }
    
    Sequence< OUString > ASIMaths_impl::getSupportedServiceNames() throw (RuntimeException)
    {
	FUNK;
	return getSupportedServiceNames_ASIMaths_impl();
    }
    
//XLocalizable
    void ASIMaths_impl::setLocale( const lang::Locale& aLocale ) throw (RuntimeException)
    {
	FUNK;
	locale = aLocale;
    }
    
    lang::Locale ASIMaths_impl::getLocale(  ) throw (RuntimeException)
    {
	FUNK;
	return locale;
    }
    
    static Reference< XInterface > SAL_CALL create_ASIMaths_impl( Reference< XComponentContext > const & xContext ) SAL_THROW( () )
    {
	FUNK;

	// gsl throws exceptions now
	gsl_set_error_handler (&handler);
	
	return static_cast< ::cppu::OWeakObject * > ( new ASIMaths_impl );
    }
    
    static struct ::cppu::ImplementationEntry s_component_entries[] =
	{
	    { create_ASIMaths_impl, getImplementationName_ASIMaths_impl,
	      getSupportedServiceNames_ASIMaths_impl, ::cppu::createSingleComponentFactory, 0, 0 }
	    ,
	    { 0, 0, 0, 0, 0, 0 }
	};
}

extern "C"
{
    void SAL_CALL component_getImplementationEnvironment( sal_Char const ** ppEnvTypeName, uno_Environment ** ppEnv )
    {
	FUNK;
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
    }
    
    sal_Bool SAL_CALL component_writeInfo( lang::XMultiServiceFactory * xMgr, registry::XRegistryKey * xRegistry )
    {
	FUNK;
	return ::cppu::component_writeInfoHelper( xMgr, xRegistry, ::_ASIMaths_impl_::s_component_entries );
    }
    
    void * SAL_CALL component_getFactory( sal_Char const * implName,
					  lang::XMultiServiceFactory * xMgr, registry::XRegistryKey * xRegistry )
    {
	FUNK;

	Reference< XInterface  > xInterface = xMgr->createInstance(OUString::createFromAscii( "com.sun.star.reflection.CoreReflection" ));
 
	Reference< XIdlReflection > rIDLReflection(xInterface, UNO_QUERY);
	_ASIMaths_impl_::ASIMaths_impl::ourIDLClass = rIDLReflection->forName(OUString::createFromAscii("org.asi.XMaths"));

	if (!_ASIMaths_impl_::ASIMaths_impl::ourIDLClass.is()) printf("Error XIdlClass\n");

	return ::cppu::component_getFactoryHelper(implName, xMgr, xRegistry, ::_ASIMaths_impl_::s_component_entries );
    }
}
