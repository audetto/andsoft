/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/utils/LabelValueBlock.h>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace
{
    template <typename T>
    class VariantConvert : public boost::static_visitor<T>
    {
    public:
        
        T operator()(const long & i) const
        {
            return boost::lexical_cast<T>(i);
        }
        
        T operator()(const double & i) const
        {
            return boost::lexical_cast<T>(i);
        }
        
        T operator()(const std::string & str) const
        {
            return boost::lexical_cast<T>(str);
        }
        
    };    
}

namespace ASI
{
    void fromASIVariant(const ASIVariant & var, long & out)
    {
        out = boost::apply_visitor(VariantConvert<long>(), var);
    }

    void fromASIVariant(const ASIVariant & var, string & out)
    {
        out = boost::apply_visitor(VariantConvert<string>(), var);
    }

    void fromASIVariant(const ASIVariant & var, double & out)
    {
        out = boost::apply_visitor(VariantConvert<double>(), var);
    }

    void fromASIVariant(const ASIVariant & var, size_t & out)
    {
        const long t = boost::apply_visitor(VariantConvert<long>(), var);
        if (t < 0)
            THROW_EXCEPTION(t << " is not a valid size_t.");
        out = t;
    }

}
