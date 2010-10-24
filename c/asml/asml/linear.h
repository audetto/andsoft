#ifndef ASI_LINEAR_H
#define ASI_LINEAR_H

#include <asml/utils.h>

namespace ASI
{
    void svdSolve(const MatrixPtr & a, const CVectorPtr & b, VectorPtr & x);
}

#endif
