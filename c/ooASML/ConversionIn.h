/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_CONVERSION_IN_H
#define ASI_CONVERSION_IN_H

#include <asml/utils/utils.h>
#include <asml/utils/LabelValueBlock.h>
#include "Cache.h"

#include <ql/time/date.hpp>

#include <com/sun/star/uno/Sequence.hxx>
#include <vector>

namespace ASI
{
    
    void ooConvertIn(const ::rtl::OUString & s1, std::string & str);
    void ooConvertIn(const ::rtl::OUString & cplStr, cpl & cplNum);
    void ooConvertIn(double val, QuantLib::Date & date);
    void ooConvertIn(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any> > & a, LVB_t & lvb);
    void ooConvertIn(const ::com::sun::star::uno::Any & a, ASIVariant & var);

    void ooConvertIn(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& mat, MatrixPtr & matPtr);
    void ooConvertIn(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<double> >& vect, VectorPtr & vectPtr);

    template <typename T>
	void ooConvertIn(const T & t, T & s)
    {
        s = t;
    }

    template <typename T>
    void ooConvertIn(const ::rtl::OUString & s, boost::shared_ptr<const T> & p)
    {
        std::string str;
        ooConvertIn(s, str);

        p = ObjectCache::instance().get<T>(str);
    }

    template <typename T, typename S>
	void ooConvertIn(const ::com::sun::star::uno::Sequence<T> & t, std::vector<S> & s)
    {
        const size_t size = t.getLength();

        s.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            ooConvertIn(t[i], s[i]);
        }
    }

    // this is necessary, otherwise Sequence<Sequence<T>> -> vector<S> matches more and gcc gets lost
    template <typename T, typename S>
    void ooConvertIn(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<T> >& ooMat, std::vector<std::vector<S> > & stdMat)
    {
        ooConvertIn< ::com::sun::star::uno::Sequence<T>, std::vector<S> >(ooMat, stdMat);
    }

    template <typename T, typename S>
    void ooConvertIn(const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Sequence<T> >& ooVect, std::vector<S> & stdVect)
    {
        std::vector<std::vector<S> > matrix;
        ooConvertIn(ooVect, matrix);

        if (matrix.empty())
        {
            stdVect.clear();
            return;
        }

        const size_t rows = matrix.size();
        const size_t cols = matrix[0].size();

        if (rows == 1)
        {
            stdVect = matrix[0];
        }
        else if (cols == 1)
        {
            stdVect.resize(rows);
            for (size_t i = 0; i < rows; ++i)
            {
                stdVect[i] = matrix[i][0];
            }
        }
        else
        {
            THROW_EXCEPTION("Invalid vector: " << rows << " x " << cols);
        }
    }
    
}

#endif
