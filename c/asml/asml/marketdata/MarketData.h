/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_MARKET_DATA_H
#define ASI_MARKET_DATA_H

#include <asml/marketdata/RawMarketData.h>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/processes/stochasticprocessarray.hpp>
#include <ql/termstructures/volatility/equityfx/blackvoltermstructure.hpp>

namespace ASI
{
    class MarketData
    {
    public:
        MarketData(const boost::shared_ptr<const RawMarketData> & rawData);

        const QuantLib::Currency & currency(const std::string & name) const;
        const QuantLib::Handle<QuantLib::YieldTermStructure> & yieldCurve(const QuantLib::Currency & currency) const;

        boost::shared_ptr<QuantLib::StochasticProcess1D> stockProcess(const std::string & name) const;

        boost::shared_ptr<QuantLib::StochasticProcessArray> stockProcess(const QuantLib::Currency & ccy, const std::vector<std::string> & names) const;
        
    private:
        void unpack();

        const boost::shared_ptr<const RawMarketData> m_rawData;

        std::map<std::string, QuantLib::Handle<QuantLib::YieldTermStructure> >        m_rateMap;
        std::map<std::string, QuantLib::Handle<QuantLib::YieldTermStructure> >        m_dividendMap;
        std::map<std::string, QuantLib::Handle<QuantLib::BlackVolTermStructure> >     m_volatilityMap;
        std::map<std::string, double>                                                 m_spotMap;
    };
}

#endif
