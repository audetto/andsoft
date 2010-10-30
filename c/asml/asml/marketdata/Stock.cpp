/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/marketdata/Stock.h>

namespace ASI
{
    Stock::Stock(double                     spotPrice,
                 double                     dividendYield,
                 double                     volatility,
                 const QuantLib::Currency & currency)
        :
        m_spotPrice(spotPrice),
        m_dividendYield(dividendYield),
        m_volatility(volatility),
        m_currency(currency)
    {
    }

    double Stock::spotPrice() const
    {
        return m_spotPrice;
    }

    double Stock::dividendYield() const
    {
        return m_dividendYield;
    }

    double Stock::volatility() const
    {
        return m_volatility;
    }

    const QuantLib::Currency & Stock::currency() const
    {
        return m_currency;
    }
    
}
