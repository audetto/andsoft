/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/marketdata/MarketData.h>
#include <asml/utils/utils.h>
#include <boost/foreach.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/termstructures/volatility/equityfx/blackconstantvol.hpp>
#include <ql/processes/blackscholesprocess.hpp>

using namespace QuantLib;
using namespace std;

namespace ASI
{
    MarketData::MarketData(const boost::shared_ptr<const RawMarketData> & rawData) :
        m_rawData(rawData)
    {
        unpack();
    }

    void MarketData::unpack()
    {
        DayCounter dayCounter = Actual365Fixed();
        Calendar calendar = TARGET();

        const RawMarketData::StockMap_t & stockMap = m_rawData->rawStocks();
        const RawMarketData::RateMap_t  & rateMap  = m_rawData->rawRates();
        const Date                      & valuationDate = m_rawData->valuationDate();

        BOOST_FOREACH(const RawMarketData::RateMap_t::value_type & it, rateMap)
        {
            const QuantLib::Currency & ccy = it.first;
            const double rate = it.second;
            Handle<YieldTermStructure> yc(boost::shared_ptr<YieldTermStructure>(new FlatForward(valuationDate, rate, dayCounter)));
            
            m_rateMap[ccy] = yc;
        }

        BOOST_FOREACH(const RawMarketData::StockMap_t::value_type & it, stockMap)
        {
            const std::string & name   = it.first;
            const Stock & stock        = *it.second;
            const double dividendYield = stock.dividendYield();
            const double volatility    = stock.volatility();
            const double spotPrice     = stock.spotPrice();

            Handle<YieldTermStructure> divs(boost::shared_ptr<YieldTermStructure>(new FlatForward(valuationDate, dividendYield, dayCounter)));            
            Handle<BlackVolTermStructure> vols(boost::shared_ptr<BlackVolTermStructure>(new BlackConstantVol(valuationDate, calendar, volatility, dayCounter)));

            m_dividendMap[name] = divs;
            m_volatilityMap[name] = vols;
            m_spotMap[name] = spotPrice;
        }
    }

    const Currency & MarketData::currency(const std::string & name) const
    {
        const RawMarketData::StockMap_t & stockMap = m_rawData->rawStocks();
        const boost::shared_ptr<const Stock> & stock = findInMap(stockMap, name);
        return stock->currency();
    }

    const Handle<YieldTermStructure> & MarketData::yieldCurve(const QuantLib::Currency & currency) const
    {
        const Handle<YieldTermStructure> & yieldCurve = findInMap(m_rateMap, currency);
        return yieldCurve;
    }
   
    boost::shared_ptr<StochasticProcess1D> MarketData::stockProcess(const std::string & name) const
    {
        const double spotPrice = findInMap(m_spotMap, name);
        const Currency & ccy = currency(name);
        const Handle<YieldTermStructure> & yc = yieldCurve(ccy);
        const Handle<YieldTermStructure> & divs = findInMap(m_dividendMap, name);
        const Handle<BlackVolTermStructure> & vols = findInMap(m_volatilityMap, name);

        Handle<Quote> underlyingH(boost::shared_ptr<Quote>(new SimpleQuote(spotPrice)));

        boost::shared_ptr<StochasticProcess1D> stochasticProcess(new BlackScholesMertonProcess(underlyingH, divs, yc, vols));
        
        return stochasticProcess;         
    }

    boost::shared_ptr<QuantLib::StochasticProcessArray> MarketData::stockProcess(const QuantLib::Currency & ccy, const std::vector<std::string> & names) const
    {
        const size_t numberOfStocks = names.size();

        vector<boost::shared_ptr<StochasticProcess1D> > arrayOfProcesses(numberOfStocks);

        for (size_t i = 0; i < numberOfStocks; ++i)
        {
            if (currency(names[i]) != ccy)
                THROW_EXCEPTION("Mixed currencies not supported yet");

            arrayOfProcesses[i] = stockProcess(names[i]);
        }
        
        Matrix corr = m_rawData->correlation()->correlation(names);

        boost::shared_ptr<StochasticProcessArray> mdProcess(new StochasticProcessArray(arrayOfProcesses, corr));

        return mdProcess;

    }
    
    const QuantLib::Date & MarketData::valuationDate() const
    {
        const Date & valuationDate = m_rawData->valuationDate();
        return valuationDate;
    }

}
