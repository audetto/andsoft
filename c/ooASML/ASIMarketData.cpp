/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "ASIMaths_impl.h"

#include <asml/marketdata/MarketData.h>
#include <asml/payoff/JSPayoff.h>
#include <asml/payoff/GenericPathOption.h>
#include <asml/montecarlo/Montecarlo.h>

#include <ql/currencies/europe.hpp>

#include "ooutils.h"
#include "ConversionOut.h"
#include "ConversionIn.h"

using namespace ::rtl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::reflection;
using namespace ::com::sun::star::uno;
using namespace ASI;
using namespace std;

namespace _ASIMaths_impl_
{

    OUString SAL_CALL ASIMaths_impl::createMarketData( const OUString& name, 
                                                       double date, 
                                                       const Sequence< Sequence< OUString > >& ooCcys, 
                                                       const Sequence< Sequence< double > >& ooRates, 
                                                       const Sequence< Sequence< OUString > >& ooStocks, 
                                                       const Sequence< Sequence< double > >& ooSpots, 
                                                       const Sequence< Sequence< OUString > >& ooDomestics, 
                                                       const Sequence< Sequence< double > >& ooRepos, 
                                                       const Sequence< Sequence< double > >& ooVols, 
                                                       const Sequence< Sequence< double > >& ooCorrelations ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        const QuantLib::Date valuationDate(date);

        std::vector<std::string> ccys;
        ooConvertIn(ooCcys, ccys);

        std::vector<double> rates;
        ooConvertIn(ooRates, rates);

        std::vector<std::string> stocks;
        ooConvertIn(ooStocks, stocks);

        std::vector<double> spots;
        ooConvertIn(ooSpots, spots);

        std::vector<std::string> domestics;
        ooConvertIn(ooDomestics, domestics);

        std::vector<double> repos;
        ooConvertIn(ooRepos, repos);

        std::vector<double> vols;
        ooConvertIn(ooVols, vols);

        std::vector<std::vector<double> > correlations;
        ooConvertIn(ooCorrelations, correlations);

        const size_t numberOfRates = ccys.size();

        if (numberOfRates != rates.size())
            THROW_EXCEPTION("Invalid size of ccy/rates: " << numberOfRates << " != " << rates.size());

        const size_t numberOfStocks = stocks.size();

        if (numberOfStocks != spots.size() || 
            numberOfStocks != domestics.size() || 
            numberOfStocks != repos.size() ||
            numberOfStocks != vols.size())
            THROW_EXCEPTION("Invalid size of stocks/domestics/repos/vols");

        if (numberOfStocks != correlations.size())
            THROW_EXCEPTION("Invalid size of correlation: " << numberOfStocks << " != " << correlations.size());

        QuantLib::EURCurrency ccy;

        RawMarketData::RateMap_t ratesMap;
        for (size_t i = 0; i < numberOfRates; ++i)
        {
            ratesMap[ccy] = rates[i];
        }

        RawMarketData::StockMap_t stocksMap; 
        for (size_t i = 0; i < numberOfStocks; ++i)
        {
            stocksMap[stocks[i]].reset(new Stock(spots[i], repos[i], vols[i], ccy));
        }

        QuantLib::Matrix corr(numberOfStocks, numberOfStocks);
        for (size_t i = 0; i < numberOfStocks; ++i)
        {
            for (size_t j = 0; j < i; ++j)
            {
                corr[i][j] = correlations[i][j];
                corr[j][i] = corr[i][j];
            }
            corr[i][i] = 1.0;
        }
        boost::shared_ptr<const Correlation> correl(new Correlation(stocks, corr));

        boost::shared_ptr<const RawMarketData> rawData(new RawMarketData(valuationDate, stocksMap, ratesMap, correl));

        boost::shared_ptr<const MarketData> marketData(new MarketData(rawData));

        string str;
        ooConvertIn(name, str);
        
        return ooDirectConvert<OUString>(std::make_pair(str, marketData));

        WRAP_END;
    }
    
    OUString SAL_CALL ASIMaths_impl::createPayoff( const OUString& name, const Sequence< Sequence< double > >& ooDates, const OUString& ooFilename ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        vector<QuantLib::Date> dates;
        ooConvertIn(ooDates, dates);

        string filename;
        ooConvertIn(ooFilename, filename);

        boost::shared_ptr<QuantLib::PathPayoff> jsPayoff(new JSPayoff(filename));
        boost::shared_ptr<const QuantLib::PathMultiAssetOption> option(new GenericPathOption(jsPayoff, dates));

        string str;
        ooConvertIn(name, str);
        
        return ooDirectConvert<OUString>(std::make_pair(str, option));
        
        WRAP_END;
    }

    Sequence<Sequence<double> > SAL_CALL ASIMaths_impl::mcPricer( const OUString& ooPayoff, const OUString & ooCcy, const Sequence< Sequence< OUString > >& ooNames, const OUString & ooMarketData ) throw (RuntimeException)
    {
        WRAP_BEGIN;

        boost::shared_ptr<const QuantLib::PathMultiAssetOption> option;
        ooConvertIn(ooPayoff, option);

        QuantLib::EURCurrency ccy;

        vector<string> names;
        ooConvertIn(ooNames, names);

        boost::shared_ptr<const MarketData> marketData;
        ooConvertIn(ooMarketData, marketData);

        MCDetails details;
        details.timeSteps = 1;
        details.paths = 1023;

        QuantLib::PathMultiAssetOption & ins = const_cast<QuantLib::PathMultiAssetOption &>(*option);

        MCResult result = priceViaMC(ins, ccy, names, marketData, details);

        vector<double> res(2);
        res[0] = result.price;
        res[1] = result.error;

        return ooDirectConvert<Sequence<Sequence<double> > >(res);

        WRAP_END;
    }
}

