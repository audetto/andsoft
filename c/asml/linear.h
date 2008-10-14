#ifndef ASI_LINEAR_H
#define ASI_LINEAR_H

#include <asi/utils.h>

namespace ASI
{
    void svdSolve(const MatrixPtr & a, const CVectorPtr & b, VectorPtr & x);
}

#endif
