/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef JS_PAYOFF_H
#define JS_PAYOFF_H

#include <ql/experimental/mcbasket/pathpayoff.hpp>

namespace ASI
{
    
    class JSPayoff : public QuantLib::PathPayoff
    {
    public:
        JSPayoff(const std::string & filename);

        virtual std::string name() const;
        virtual std::string description() const;
        virtual void value(const QuantLib::Matrix                                             & path, 
                           const std::vector<QuantLib::Handle<QuantLib::YieldTermStructure> > & forwardTermStructures,
                           QuantLib::Array                                                    & payments, 
                           QuantLib::Array                                                    & exercises, 
                           std::vector<QuantLib::Array>                                       & states) const;
        virtual QuantLib::Size basisSystemDimension() const;

        std::vector<QuantLib::Date> fixingDates() const;

    private:

        struct PImpl;
        std::auto_ptr<PImpl> m_pimpl;
    };
    
}

#endif
