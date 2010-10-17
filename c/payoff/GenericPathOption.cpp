/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "GenericPathOption.h"

using namespace QuantLib;

namespace AndSoft
{
    
    GenericPathOption::GenericPathOption(const boost::shared_ptr<PathPayoff>        & pathPayoff,
                                         const std::vector<Date>                    & fixingDates,
                                         const boost::shared_ptr<PricingEngine>     & engine) :
        PathMultiAssetOption(engine),
        m_pathPayoff(pathPayoff),
        m_fixingDates(fixingDates)
    {
    }
    
    boost::shared_ptr<PathPayoff> GenericPathOption::pathPayoff()  const
    {
        return m_pathPayoff;
    }
    
    std::vector<Date>             GenericPathOption::fixingDates() const
    {
        return m_fixingDates;
    }
    
}
