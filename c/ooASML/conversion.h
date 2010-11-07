/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_CONVERSION_H
#define ASI_CONVERSION_H

#include <asml/utils/utils.h>

#include <com/sun/star/uno/Sequence.hxx>
#include <vector>

namespace ASI
{
    
    template <typename T, typename S>
	void ooConvert(const T & t, S & s);
    
    template <typename T>
	void ooConvert(const T & t, T & s)
    {
        s = t;
    }

    template <typename T, typename S>
	void ooConvert(const ::com::sun::star::uno::Sequence<T> & t, std::vector<S> & s)
    {
        const size_t size = t.getLength();

        s.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            ooConvert(t[i], s[i]);
        }
    }

    template <typename T, typename S>
	void ooConvert(const std::vector<T> & t, ::com::sun::star::uno::Sequence<S> & s)
    {
        const size_t size = t.size();

        s = ::com::sun::star::uno::Sequence<S>(size);

        for (size_t i = 0; i < size; ++i)
        {
            ooConvert(t[i], s[i]);
        }
    }

    template <>
    void ooConvert(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect, std::vector<double> & stdVect);

    template <>
    void ooConvert(const std::vector<double> & stdVect, ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect);

    template <>
    void ooConvert(const ::com::sun::star::uno::Sequence<double> & vect, cpl & cplNum);

    template <>
    void ooConvert(const cpl & cplNum, ::com::sun::star::uno::Sequence<double> & vect);


    template <typename S, typename T>
	S ooDirectConvert(const T & t)
    {
        S s;
        ooConvert(t, s);
        return s;
    }

    void appendStdVectorToOOArgument(::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> > & seq, const std::vector<double> & vect);

    
}

#endif
