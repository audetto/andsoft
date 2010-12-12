/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/payoff/JSPathOption.h>

using namespace QuantLib;

namespace ASI
{
    
    JSPathOption::JSPathOption(const std::string                        & jsPayoff,
                               const boost::shared_ptr<PricingEngine>   & engine) :
        PathMultiAssetOption(engine),
        m_jsPayoff(new JSPayoff(jsPayoff))
    {
    }
    
    boost::shared_ptr<PathPayoff> JSPathOption::pathPayoff()  const
    {
        return m_jsPayoff;
    }
    
    std::vector<Date>             JSPathOption::fixingDates() const
    {
        return m_jsPayoff->fixingDates();
    }
    
}
