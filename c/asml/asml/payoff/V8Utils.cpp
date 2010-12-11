/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/payoff/V8Utils.h>

using namespace v8;

namespace ASI
{
    void v8ConvertIn(const Handle<Value> & value, QuantLib::Date & date)
    {
        if (value->IsNumber())
        {
            const double val = value->NumberValue();
            date = QuantLib::Date(val);
        }
        else
        {
            THROW_EXCEPTION("value is not a number");
        }
    }
    
}
