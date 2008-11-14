#ifndef ASI_FAST_EXP_H
#define ASI_FAST_EXP_H

#include <asi/utils.h>

namespace ASI
{
  MatrixPtr expViaEigenvalues(const double time, MatrixPtr & mat);
  MatrixPtr expViaGSL        (const double time, const MatrixPtr & mat);
  MatrixPtr expViaExplicit   (const double time, const MatrixPtr & mat, const size_t powerOfTwo);
}

#endif
