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
