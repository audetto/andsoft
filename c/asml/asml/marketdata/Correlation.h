/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_CORRELATION_H
#define ASI_CORRELATION_H

#include <ql/math/matrix.hpp>
#include <map>

namespace ASI
{
    class Correlation
    {
    public:
        Correlation(const std::vector<std::string> & names,
                    QuantLib::Matrix               & values);

        double correlation(const std::string & first, const std::string & second) const;
        QuantLib::Disposable<QuantLib::Matrix> correlation(const std::vector<std::string> & names) const;

    private:
        typedef std::map<std::pair<std::string, std::string>, double> CorrelationMap_t;

        CorrelationMap_t m_values;
    };
}

#endif
