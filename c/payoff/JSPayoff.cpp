/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "JSPayoff.h"

using namespace QuantLib;

namespace AndSoft
{
    
    JSPayoff::JSPayoff()
    {
        
    }
    
    std::string JSPayoff::name() const
    {
        return "JSPayoff";
    }
    
    std::string JSPayoff::description() const
    {
        return name();
    }
    
    Size JSPayoff::basisSystemDimension() const
    {
        return 2;
    }
    
    void JSPayoff::value(const Matrix       & path, 
                         const std::vector<Handle<YieldTermStructure> > & forwardTermStructures,
                         Array              & payments, 
                         Array              & exercises, 
                         std::vector<Array> & states) const
    {
        
    }
    
}
