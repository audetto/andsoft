/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef V8_UTILS_H
#define V8_UTILS_H

#include <asml/utils/error.h>
#include <v8.h>
#include <ql/time/date.hpp>
#include <vector>

namespace ASI
{
    void v8ConvertIn(const v8::Handle<v8::Value> & value, QuantLib::Date & date);

    template<typename T>
    void v8ConvertIn(const v8::Handle<v8::Value> & value, std::vector<T> & v)
    {
        if (!value->IsArray())
            THROW_EXCEPTION("V8: Not an array");

        v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(value);
        const size_t size = arr->Length();

        v.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            v8::Handle<v8::Value> val = arr->Get(i);
            v8ConvertIn(val, v[i]);
        }

    }

}

#endif
