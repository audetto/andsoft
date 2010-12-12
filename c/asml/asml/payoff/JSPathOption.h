/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef JS_PATH_OPTION_H
#define JS_PATH_OPTION_H

#include <ql/experimental/mcbasket/pathmultiassetoption.hpp>
#include <asml/payoff/JSPayoff.h>

namespace ASI
{

    // The option
    
    class JSPathOption : public QuantLib::PathMultiAssetOption
    {
    public:
        JSPathOption(const std::string                                 & jsPayoff,
                     const boost::shared_ptr< QuantLib::PricingEngine> & engine = boost::shared_ptr< QuantLib::PricingEngine>() );
        
        virtual boost::shared_ptr< QuantLib::PathPayoff> pathPayoff()     const;
        virtual std::vector< QuantLib::Date>             fixingDates()    const;
    private:
        boost::shared_ptr< JSPayoff> m_jsPayoff;
    };
    
}

#endif
