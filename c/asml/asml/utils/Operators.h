/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_OPERATOR_H
#define ASI_OPERATOR_H

#include <ql/currency.hpp>

namespace QuantLib
{
    bool operator<(const QuantLib::Currency & lhs, const QuantLib::Currency & rhs);
}

#endif
