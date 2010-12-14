/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef EVENT_PAYOFF_H
#define EVENT_PAYOFF_H

#include <asml/payoff/JSPathOption.h>

namespace ASI
{
    boost::shared_ptr<JSPathOption> createEventBasedPayoff(const std::vector<std::string>               & includes,
                                                           const std::vector<QuantLib::Date>            & dates,
                                                           const std::vector<std::vector<std::string> > & events);    
}

#endif
