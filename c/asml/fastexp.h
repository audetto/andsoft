#ifndef ASI_FAST_EXP_H
#define ASI_FAST_EXP_H

#include <asi/utils.h>

namespace ASI
{
    MatrixPtr expViaEigenvalues(const double time, MatrixPtr & mat);
    MatrixPtr expViaGSL        (const double time, const MatrixPtr & mat);
    MatrixPtr expViaExplicit   (const double time, const MatrixPtr & mat, const size_t powerOfTwo);
    
    // theta = 0.0 explicit
    //         0.5 CN
    //         1.0 implicit
    MatrixPtr expViaTheta      (const double time, const MatrixPtr & mat, const size_t powerOfTwo, const double theta);
    
    void fastexp_try();
}

#endif
