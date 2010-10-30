/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/marketdata/RawMarketData.h>

namespace ASI
{
    RawMarketData::RawMarketData(const QuantLib::Date & valuationDate,
                                 const StockMap_t & rawStocks,
                                 const RateMap_t & rawRates,
                                 const boost::shared_ptr<const Correlation> & correl) :
        m_valuationDate(valuationDate),
        m_rawStocks(rawStocks),
        m_rawRates(rawRates),
        m_correl(correl)
    {
    }
    
    const QuantLib::Date & RawMarketData::valuationDate() const
    {
        return m_valuationDate;
    }

    const RawMarketData::StockMap_t & RawMarketData::rawStocks() const
    {
        return m_rawStocks;
    }

    const RawMarketData::RateMap_t & RawMarketData::rawRates() const
    {
        return m_rawRates;
    }

    const boost::shared_ptr<const Correlation> & RawMarketData::correlation() const
    {
        return m_correl;
    }
  
}
