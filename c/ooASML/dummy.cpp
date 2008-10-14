#include "projection.h"

using namespace ASI;

int main()
{

    try
    {
	MatrixPtr m(gsl_matrix_calloc(3, 2), MatrixDeleter());
	VectorPtr v(gsl_vector_calloc(5), VectorDeleter());
	VectorPtr v1(gsl_vector_calloc(5), VectorDeleter());
    }
    catch (...)
    {
    }
    return 0;
}
