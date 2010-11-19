/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/montecarlo/Montecarlo.h>

#include <ql/experimental/mcbasket/mcpathbasketengine.hpp>

using namespace std;

namespace ASI
{

    MCResult priceViaMC(QuantLib::Instrument                      & instrument,
                        const QuantLib::Currency                  & ccy,
                        const vector<string>                      & names,
                        const boost::shared_ptr<const MarketData> & marketData,
                        const MCDetails                           & details)
    {
        QuantLib::Settings::instance().evaluationDate() = marketData->valuationDate();

        boost::shared_ptr<QuantLib::StochasticProcessArray> process = marketData->stockProcess(ccy, names);

        boost::shared_ptr<QuantLib::PricingEngine> mcengine = QuantLib::MakeMCPathBasketEngine<QuantLib::LowDiscrepancy>(process).withSteps(details.timeSteps).withSamples(details.paths);
        instrument.setPricingEngine(mcengine);

        MCResult result;
        result.price = instrument.NPV();
        result.error = 0.0; //instrument.errorEstimate();

        return result;
    }
}
