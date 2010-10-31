/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

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


#include <ASIMaths.h>

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
