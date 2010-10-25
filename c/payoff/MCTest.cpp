/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

// the only header you need to use QuantLib
#include <ql/quantlib.hpp>

#include <asml/payoff/AmericanLookback.h>
#include <asml/payoff/GenericPathOption.h>
#include <asml/payoff/JSPayoff.h>

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
        Calendar calendar = TARGET();
        Real underlying = 1.0;
        Spread dividendYield = 0.00;
        Rate riskFreeRate = 0.05;
        Volatility volatility = 0.20;
        
        Date todaysDate(15, May, 1998);
        Date settlementDate(17, May, 1998);
        Settings::instance().evaluationDate() = todaysDate;
        
        DayCounter dayCounter = Actual365Fixed();
        
        std::cout << "Underlying price = "        << underlying << std::endl;
        std::cout << "Risk-free interest rate = " << io::rate(riskFreeRate)
                  << std::endl;
        std::cout << "Dividend yield = " << io::rate(dividendYield)
                  << std::endl;
        std::cout << "Volatility = " << io::volatility(volatility)
                  << std::endl;
        std::cout << std::endl;
        
        std::string method;
        
        std::cout << std::endl ;
        
        Handle<Quote> underlyingH(boost::shared_ptr<Quote>(new SimpleQuote(underlying)));
        
        // bootstrap the yield/dividend/vol curves
        Handle<YieldTermStructure> flatTermStructure(
                                                     boost::shared_ptr<YieldTermStructure>(
                                                                                           new FlatForward(settlementDate, riskFreeRate, dayCounter)));
        Handle<YieldTermStructure> flatDividendTS(
                                                  boost::shared_ptr<YieldTermStructure>(
                                                                                        new FlatForward(settlementDate, dividendYield, dayCounter)));
        Handle<BlackVolTermStructure> flatVolTS(
                                                boost::shared_ptr<BlackVolTermStructure>(new BlackConstantVol(settlementDate, calendar, volatility, dayCounter)));
        
        boost::shared_ptr<StochasticProcess1D> stochasticProcess(
                                                                 new BlackScholesMertonProcess(underlyingH, flatDividendTS, flatTermStructure, flatVolTS));
        std::vector<boost::shared_ptr<StochasticProcess1D> > arrayOfProcesses;
        arrayOfProcesses.push_back(stochasticProcess);
        
        Matrix corr(1, 1);
        corr[0][0] = 1;
        boost::shared_ptr<StochasticProcessArray> mdProcess(new StochasticProcessArray(arrayOfProcesses, corr));
        
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
        
        method = "MC (Sobol)";
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
