/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_CONVERSION_H
#define ASI_CONVERSION_H

#include <asml/utils/utils.h>

#include <com/sun/star/uno/Sequence.hxx>
#include <vector>

namespace ASI
{
    
    void ooConvert(const ::rtl::OUString & s1, std::string & str);
    void ooConvert(const ::com::sun::star::uno::Sequence<double> & vect, cpl & cplNum);

    void ooConvert(const std::string & str, ::rtl::OUString & s1);
    void ooConvert(const cpl & cplNum, ::com::sun::star::uno::Sequence<double> & vect);

    void ooConvert(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& mat, MatrixPtr & matPtr);
    void ooConvert(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect, VectorPtr & vectPtr);
    void ooConvert(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect, std::vector<double> & stdVect);

    void ooConvert(const MatrixPtr & matPtr, ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> > & mat);
    void ooConvert(const VectorPtr & vectPtr, ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect);
    void ooConvert(double val, ::com::sun::star::uno::Sequence<double> & vect);

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
