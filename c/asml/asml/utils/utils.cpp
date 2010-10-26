/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/utils/utils.h>
#include <asml/utils/error.h>
#include <iostream>

using namespace std;

namespace ASI
{
    
    void MatrixDeleter::operator()(gsl_matrix * matrix) const
    {
        if (matrix)
            gsl_matrix_free(matrix);
    }
    
    void VectorDeleter::operator()(gsl_vector * vector) const
    {
        if (vector)
            gsl_vector_free(vector);
    }
    
    void handler (const char * reason, const char * file, int line, int gsl_errno)
    {
        cerr << "GSL ERROR: " << reason << " (" << file << ":" << line << ")" << endl;
        error(reason);
    }
    
    const cpl I(0.0, 1.0);
}
