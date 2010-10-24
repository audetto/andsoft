#ifndef ASI_FAST_EXP_H
#define ASI_FAST_EXP_H

#include <asml/utils.h>

namespace ASI
{
    MatrixPtr expViaEigenvalues(const double time, const MatrixPtr & mat);
    MatrixPtr expViaGSL        (const double time, const CMatrixPtr & mat);
    MatrixPtr expViaExplicit   (const double time, const CMatrixPtr & mat, const size_t powerOfTwo);
    
    // theta = 0.0 explicit
    //         0.5 CN
    //         1.0 implicit
    MatrixPtr expViaTheta      (const double time, const CMatrixPtr & mat, const size_t powerOfTwo, const double theta);

	// traditional implicit scheme applied to rhs
    VectorPtr expViaTraditional(const double time, const MatrixPtr & mat, const size_t powerOfTwo, const VectorPtr & rhs);
    
    void fastexp_try();
}

#endif
