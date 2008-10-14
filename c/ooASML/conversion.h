#ifndef ASI_CONVERSION_H
#define ASI_CONVERSION_H

#include "utils.h"

#include <com/sun/star/uno/Sequence.hxx>
#include <vector>

namespace ASI
{
    using namespace ::com::sun::star::uno;

    MatrixPtr matrixFromOOArgument(const Sequence<Sequence<double> >& mat);
    Sequence<Sequence<double> > matrixToOOArgument(const CMatrixPtr & mat);

    VectorPtr vectorFromOOArgument(const Sequence<Sequence<double> >& vect);
    Sequence<Sequence<double> > vectorToOOArgument(const CVectorPtr & vect);

    std::vector<double> stdVectorFromOOArgument(const Sequence<Sequence<double> >& vect);
    Sequence<Sequence<double> > stdVectorToOOArgument(const std::vector<double> & vect);
}

#endif
