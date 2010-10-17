/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef JS_PAYOFF_H
#define JS_PAYOFF_H

#define BOOST_LIB_DIAGNOSTIC
#  include <ql/quantlib.hpp>
#undef BOOST_LIB_DIAGNOSTIC

namespace AndSoft
{
    
    class JSPayoff : public QuantLib::PathPayoff
    {
    public:
        JSPayoff();
        virtual std::string name() const;
        virtual std::string description() const;
        virtual void value(const QuantLib::Matrix                                             & path, 
                           const std::vector<QuantLib::Handle<QuantLib::YieldTermStructure> > & forwardTermStructures,
                           QuantLib::Array                                                    & payments, 
                           QuantLib::Array                                                    & exercises, 
                           std::vector<QuantLib::Array>                                       & states) const;
        virtual QuantLib::Size basisSystemDimension() const;
    private:
    };
    
}

#endif
