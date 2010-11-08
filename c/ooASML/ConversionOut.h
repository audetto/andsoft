/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_CONVERSION_OUT_H
#define ASI_CONVERSION_OUT_H

#include <asml/utils/utils.h>

#include <com/sun/star/uno/Sequence.hxx>
#include <vector>

namespace ASI
{
    
    void ooConvertOut(const std::string & str, ::rtl::OUString & s1);
    void ooConvertOut(const cpl & cplNum, ::rtl::OUString & cplStr);

    void ooConvertOut(const MatrixPtr & matPtr, ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> > & mat);
    void ooConvertOut(const VectorPtr & vectPtr, ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect);

    template <typename T>
	void ooConvertOut(const T & t, T & s)
    {
        s = t;
    }

    // forward declared since this is used by the T -> Sequence<S> conversion
    // and the other way round
    template <typename T, typename S>
	void ooConvertOut(const std::vector<T> & t, ::com::sun::star::uno::Sequence<S> & s);

    template <typename T, typename S>
    void ooConvertOut(const T & val, ::com::sun::star::uno::Sequence<S> & vect)
    {
        std::vector<T> v(1, val);
        ooConvertOut(v, vect);
    }

    template <typename T, typename S>
	void ooConvertOut(const std::vector<T> & t, ::com::sun::star::uno::Sequence<S> & s)
    {
        const size_t size = t.size();

        s = ::com::sun::star::uno::Sequence<S>(size);

        for (size_t i = 0; i < size; ++i)
        {
            ooConvertOut(t[i], s[i]);
        }
    }

    template <typename S, typename T>
	S ooDirectConvert(const T & t)
    {
        S s;
        ooConvertOut(t, s);
        return s;
    }

    void appendStdVectorToOOArgument(::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> > & seq, const std::vector<double> & vect);

}

#endif
