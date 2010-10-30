/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_STOCK_H
#define ASI_STOCK_H

#include <ql/currency.hpp>

namespace ASI
{
    /*
      Raw Market Data with no analytics attached
     */

    class Stock
    {
    public:
        Stock(double                     spotPrice,
              double                     dividendYield,
              double                     volatility,
              const QuantLib::Currency & currency);

        double                     spotPrice() const;
        double                     dividendYield() const;
        double                     volatility() const;
        const QuantLib::Currency & currency() const;

    private:
        const double             m_spotPrice;
        const double             m_dividendYield;
        const double             m_volatility;
        const QuantLib::Currency m_currency;
    };
}

#endif
