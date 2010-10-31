/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "ASIMaths_impl.h"

#include <asml/utils/utils.h>

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::reflection;
using namespace ::com::sun::star::uno;

namespace _ASIMaths_impl_
{
    Reference< XIdlClass > ASIMaths_impl::ourIDLClass;
    
#define _serviceName_ "org.asi.Maths"
    
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
        gsl_set_error_handler (&ASI::handler);
        
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
        
        if (!_ASIMaths_impl_::ASIMaths_impl::ourIDLClass.is())
            printf("Error while getting org.asi.XMaths\n");
        
        return ::cppu::component_getFactoryHelper(implName, xMgr, xRegistry, ::_ASIMaths_impl_::s_component_entries );
    }
}
