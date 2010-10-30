/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef AMERICAN_LOOKBACK_H
#define AMERICAN_LOOKBACK_H

#include <ql/experimental/mcbasket/adaptedpathpayoff.hpp>
#include <ql/indexes/iborindex.hpp>

namespace ASI
{

    class AmericanLookback : public QuantLib::AdaptedPathPayoff
    {
    public:
        AmericanLookback();
        virtual std::string name() const;
        virtual std::string description() const;
        virtual void operator()(ValuationData & data) const;
        virtual QuantLib::Size basisSystemDimension() const;
    private:
        mutable QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> m_termStructure;
        boost::shared_ptr<QuantLib::IborIndex> m_euribor3m;
    };
    
    
}

#endif
