/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/montecarlo/Montecarlo.h>

#include <ql/experimental/mcbasket/mcpathbasketengine.hpp>

using namespace std;

namespace
{

    using namespace ASI;

    LabelValueBlock<MCDetails> createLVBMCDetails()
    {
        LabelValueBlock<MCDetails> lvb_details;
        lvb_details.add("timeSteps", makeConverter(&MCDetails::timeSteps));
        lvb_details.add("paths",     makeConverter(&MCDetails::paths));
        return lvb_details;
    }
    
}


namespace ASI
{

    MCDetails::MCDetails(const LVB_t & lvb)
    {
        static LabelValueBlock<MCDetails> lvb_details = createLVBMCDetails();

        lvb_details.process(lvb, *this);        
    }

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

        typedef QuantLib::MCPathBasketEngine<QuantLib::LowDiscrepancy> PathEngine_type;

        // access std dev from the stats, since it is not normally available for Sobol numbers.
        boost::shared_ptr<PathEngine_type> pathengine = boost::dynamic_pointer_cast<PathEngine_type>(mcengine);
        const PathEngine_type::stats_type & stats = pathengine->sampleAccumulator();

        result.error = stats.errorEstimate();
        

        return result;
    }
}
