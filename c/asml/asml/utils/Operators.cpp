/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/utils/Operators.h>

namespace QuantLib
{
    bool operator<(const QuantLib::Currency & lhs, const QuantLib::Currency & rhs)
    {
        return lhs.numericCode() < rhs.numericCode();
    }
}
