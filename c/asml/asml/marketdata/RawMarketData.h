/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_RAW_MARKET_DATA_H
#define ASI_RAW_MARKET_DATA_H

#include <asml/marketdata/Stock.h>
#include <asml/marketdata/Correlation.h>
#include <ql/time/date.hpp>
#include <ql/currency.hpp>
#include <map>

namespace ASI
{
    class RawMarketData
    {
    public:
        typedef std::map<std::string, boost::shared_ptr<const Stock> > StockMap_t;
        typedef std::map<std::string, double> RateMap_t;

        RawMarketData(const QuantLib::Date & valuationDate,
                      const StockMap_t & rawStocks,
                      const RateMap_t & rawRates,
                      const boost::shared_ptr<const Correlation> & correl);

        const QuantLib::Date & valuationDate() const;
        const StockMap_t & rawStocks() const;
        const RateMap_t  & rawRates() const;
        const boost::shared_ptr<const Correlation> & correlation() const;

    private:
        const QuantLib::Date m_valuationDate;
        const StockMap_t m_rawStocks;
        const RateMap_t m_rawRates;
        const boost::shared_ptr<const Correlation> m_correl;
    };
}

#endif
