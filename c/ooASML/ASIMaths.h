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

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::reflection;
using namespace ::com::sun::star::uno;

#define FUNK  /* printf("%s\n", __func__) */

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
        Sequence<Sequence<double> > SAL_CALL projection( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException);
        Sequence<Sequence<double> > SAL_CALL dykstra( const Sequence<Sequence<double> > & x, const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException);
	Sequence<Sequence<double> > SAL_CALL svdSolve( const Sequence<Sequence<double> > & a, const Sequence<Sequence<double> > & b) throw (RuntimeException);

	Sequence<Sequence<double> > SAL_CALL fastExp( double t, const Sequence<Sequence<double> > & a, sal_Int32 method, double theta) throw (RuntimeException);

	double SAL_CALL finiteDifference( const Sequence<Sequence<double> > & xa, const Sequence<Sequence<double> > & ya, double x, sal_Int32 order) throw (RuntimeException);
	
	double SAL_CALL heston( double strike, double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 points) throw (RuntimeException);

	double SAL_CALL black( double strike, double time, double sigma) throw (RuntimeException);

	double SAL_CALL impliedVolatility( double price, double strike, double time ) throw (RuntimeException);

	Sequence<Sequence<double> > SAL_CALL variousBS( double time, double strike, double sigma) throw (RuntimeException);

	Sequence<Sequence<double> > SAL_CALL fft( const Sequence<Sequence<double> > & data) throw (RuntimeException);
	Sequence<Sequence<double> > SAL_CALL ifft( const Sequence<Sequence<double> > & data) throw (RuntimeException);
	Sequence<Sequence<double> > SAL_CALL fft_unpack( const Sequence<Sequence<double> > & data) throw (RuntimeException);
	Sequence<Sequence<double> > SAL_CALL fft_pack( const Sequence<Sequence<double> > & data) throw (RuntimeException);

	Sequence<Sequence<double> > SAL_CALL hestonFFT( double time1, double time2, double sigma, double kappa, double theta, double alpha, double rho, sal_Int32 N, double stdDev) throw (RuntimeException);

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

}

extern "C"
{
    void SAL_CALL component_getImplementationEnvironment( sal_Char const ** ppEnvTypeName, uno_Environment ** ppEnv );
    
    sal_Bool SAL_CALL component_writeInfo( lang::XMultiServiceFactory * xMgr, registry::XRegistryKey * xRegistry );
    
    void * SAL_CALL component_getFactory( sal_Char const * implName,
					  lang::XMultiServiceFactory * xMgr, registry::XRegistryKey * xRegistry );
}
