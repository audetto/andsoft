/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_LABEL_VALUE_BLOCK_H
#define ASI_LABEL_VALUE_BLOCK_H

#include <asml/utils/utils.h>
#include <string>
#include <boost/foreach.hpp>
#include <boost/variant.hpp>

namespace ASI
{
    typedef boost::variant<long, double, std::string> ASIVariant;

    template <typename C>
    class LVBConvert
    {
    public:
        virtual void convert(const ASIVariant & input, C & container) const = 0;
        virtual ~LVBConvert();
    };

    template <typename C, typename T>
    class LVB_T_Convert : public LVBConvert<C>
    {
    public:
        LVB_T_Convert(T C::* element) : m_Element(element)
        {
        }

        virtual void convert(const ASIVariant & input, C & container) const
        {
            container.*m_Element = T();
        }
    private:
        T C::* m_Element;
    };

    template <typename C, typename T>
    boost::shared_ptr<LVBConvert<C> > makeConverter(T C::* elem)
    {
        boost::shared_ptr<LVBConvert<C> > result(new LVB_T_Convert<C, T>(elem));
        return result;
    }

    typedef std::map<std::string, ASIVariant> LVB_t;

    template <typename C>
    class LabelValueBlock
    {
    private:
        typedef std::map<std::string, boost::shared_ptr<LVBConvert<C> > > ElementsMap_t;
    public:
        void add(const std::string & name, const boost::shared_ptr<LVBConvert<C> > & element)
        {
            m_Elements[name] = element;
        }

        void process(const LVB_t & values, C & container) const
        {
            BOOST_FOREACH(const typename ElementsMap_t::value_type & it, m_Elements)
            {
                const std::string & name = it.first;
                const ASIVariant & value = findInMap(values, name);

                const boost::shared_ptr<LVBConvert<C> > & converter = it.second;
                converter->convert(value, container);
            }
        }
    private:
        ElementsMap_t m_Elements;
    };
}

#endif
