/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

// the only header you need to use QuantLib
#include <ql/quantlib.hpp>

#include <asml/payoff/AmericanLookback.h>
#include <asml/payoff/GenericPathOption.h>
#include <asml/payoff/JSPayoff.h>
#include <asml/marketdata/MarketData.h>

#include <boost/timer.hpp>
#include <iostream>
#include <iomanip>

using namespace QuantLib;
using namespace std;

namespace ASI
{
  
    void mcTest()
    {
        
        boost::timer timer;
        std::cout << std::endl;
        
        // our options
        Real underlying = 1.0;
        Spread dividendYield = 0.00;
        Rate riskFreeRate = 0.05;
        Volatility volatility = 0.20;
        
        const string name = "ABC";

        EURCurrency ccy;

        RawMarketData::StockMap_t stocks;
        stocks[name].reset(new Stock(underlying, dividendYield, volatility, ccy));

        RawMarketData::RateMap_t rates;
        rates[ccy] = riskFreeRate;

        Matrix corr(1, 1);
        corr[0][0] = 1;
        vector<string> names(1, name);
        boost::shared_ptr<const Correlation> correlation(new Correlation(names, corr));

        Date todaysDate(15, May, 1998);
        Settings::instance().evaluationDate() = todaysDate;

        boost::shared_ptr<const RawMarketData> rawMarketData(new RawMarketData(todaysDate, stocks, rates, correlation));

        MarketData marketData(rawMarketData);

        boost::shared_ptr<StochasticProcessArray> mdProcess(marketData.stockProcess(ccy, names));
        
        // options
        
        boost::shared_ptr<PathPayoff> lookbackPayoff1(new AmericanLookback());
        boost::shared_ptr<PathPayoff> lookbackPayoff2(new JSPayoff("script.js"));
        std::vector<Date> fixings;
        
        Date firstFixing(17, May, 2000);
        fixings.push_back(firstFixing);
        Period annual(Annual);
        for (Size i = 0; i < 5; ++i)
            fixings.push_back(fixings.back() + annual);
        
        GenericPathOption lookbackOption1(lookbackPayoff1, fixings);
        GenericPathOption lookbackOption2(lookbackPayoff2, fixings);
        
        // Monte Carlo Method
        
        Size timeSteps = 10;
        
        const string method = "MC (Sobol)";
        Size nSamples = 1 << 15;
        Size calibSamples = 1 << 10;
        
        boost::shared_ptr<PricingEngine> mcengine;
        
        mcengine = MakeMCPathBasketEngine<LowDiscrepancy>(mdProcess).withSteps(timeSteps).withSamples(nSamples);
        lookbackOption1.setPricingEngine(mcengine);
        std::cout << method << " E [C++]  " << nSamples << " iterations: " << lookbackOption1.NPV() << std::endl;

        lookbackOption2.setPricingEngine(mcengine);
        std::cout << method << " E [JSV8] " << nSamples << " iterations: " << lookbackOption2.NPV() << std::endl;
        
        mcengine = MakeMCAmericanPathEngine<LowDiscrepancy>(mdProcess).withSteps(timeSteps).withSamples(nSamples).withCalibrationSamples(calibSamples);
        lookbackOption1.setPricingEngine(mcengine);
        std::cout << method << " A [C++]  " << nSamples << " iterations: " << lookbackOption1.NPV() << std::endl;
    }

}
