/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/marketdata/Correlation.h>
#include <asml/utils/utils.h>

using namespace QuantLib;
using namespace std;

namespace ASI
{
    Correlation::Correlation(const vector<string> & names,
                             Matrix               & values)
    {
        const size_t numberOfAssets = names.size();
        if (values.rows() != numberOfAssets || values.columns() != numberOfAssets)
            THROW_EXCEPTION("Correlation: expected " << numberOfAssets << ", found: " <<
                            values.rows() << "x" << values.columns());

        for (size_t i = 0; i < numberOfAssets; ++i)
        {
            for (size_t j = 0; j < i; ++j)
            {
                const pair<string, string> can = makeCanonical(names[i], names[j]);
                if (values[i][j] != values[j][i])
                    THROW_EXCEPTION("Unsymmetric matrix at (" << i << ", " << j << ")");
                m_values[can] = values[i][j];
            }
        }
    }

    double Correlation::correlation(const std::string & first, const std::string & second) const
    {
        if (first == second)
            return 1.0;

        const CorrelationMap_t::const_iterator it = m_values.find(makeCanonical(first, second));
        if (it == m_values.end())
        {
            THROW_EXCEPTION("Missing correlation for " << first << " / " << second);
        }

        const double rho = it->second;

        return rho;
    }

    QuantLib::Disposable<QuantLib::Matrix> Correlation::correlation(const std::vector<std::string> & names) const
    {
        const size_t numberOfStocks = names.size();
        Matrix corr(numberOfStocks, numberOfStocks);

        for (size_t i = 0; i < numberOfStocks; ++i)
        {
            for (size_t j = 0; j < i; ++j)
            {
                corr[i][j] = correlation(names[i], names[j]);
                corr[j][i] = corr[i][j];
            }
            corr[i][i] = 1.0;
        }

        return corr;
    }

}
