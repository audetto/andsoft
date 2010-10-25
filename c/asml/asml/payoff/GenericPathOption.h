/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef GENERIC_PATH_OPTION_H
#define GENERIC_PATH_OPTION_H

#include <ql/quantlib.hpp>

namespace ASI
{

    // The option
    
    class GenericPathOption : public QuantLib::PathMultiAssetOption
    {
    public:
        GenericPathOption(const boost::shared_ptr< QuantLib::PathPayoff>    & pathPayoff,
                          const std::vector< QuantLib::Date>                & fixingDates,
                          const boost::shared_ptr< QuantLib::PricingEngine> & engine = boost::shared_ptr< QuantLib::PricingEngine>() );
        
        virtual boost::shared_ptr< QuantLib::PathPayoff> pathPayoff()     const;
        virtual std::vector< QuantLib::Date>             fixingDates()    const;
    private:
        boost::shared_ptr< QuantLib::PathPayoff> m_pathPayoff;
        std::vector< QuantLib::Date>             m_fixingDates;
    };
    
}

#endif
