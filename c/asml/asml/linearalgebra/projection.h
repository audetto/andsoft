#ifndef ASI_PROJECTION_H
#define ASI_PROJECTION_H

#include <asml/utils/utils.h>

namespace ASI
{
    // project x onto {a' y = b}
    void projectOnSubspace(gsl_vector * x, const gsl_matrix * a, const gsl_vector * b);

    // return is whether the point has been changed
    bool projectOnHalf1DSubspace(gsl_vector * x, const gsl_vector * a, const double b);

    // project x onto {a' y <= b}
    void dykstraProjection(gsl_vector * x, const gsl_matrix * a, const gsl_vector * b, const size_t iter, const double tol);
}


#endif
