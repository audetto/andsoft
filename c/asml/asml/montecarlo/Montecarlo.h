/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_MONTE_CARLO_H
#define ASI_MONTE_CARLO_H

#include <ql/instrument.hpp>
#include <asml/marketdata/MarketData.h>
#include <asml/utils/LabelValueBlock.h>

namespace ASI
{

    struct MCResult
    {
        double price;
        double error;
    };

    struct MCDetails
    {
        size_t timeSteps;
        size_t paths;

        MCDetails(const LVB_t & lvb);
    };

    MCResult priceViaMC(QuantLib::Instrument                      & instrument,
                        const QuantLib::Currency                  & ccy,
                        const std::vector<std::string>            & names,
                        const boost::shared_ptr<const MarketData> & marketData,
                        const MCDetails                           & details);
  
}

#endif
