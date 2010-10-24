#include <asml/linear.h>

#include <gsl/gsl_linalg.h>

namespace ASI
{
    void svdSolve(const MatrixPtr & a, const CVectorPtr & b, VectorPtr & x)
    {
	const size_t rows = a->size1;   // M
	const size_t cols = a->size2;   // N

	if (b->size != rows)
	    error("Bad size\n");

	MatrixPtr V(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	VectorPtr S(gsl_vector_alloc(cols), VectorDeleter());
	VectorPtr work(gsl_vector_alloc(cols), VectorDeleter());

	x = VectorPtr(gsl_vector_alloc(cols), VectorDeleter());

	gsl_linalg_SV_decomp (a.get(), V.get(), S.get(), work.get());

	gsl_linalg_SV_solve (a.get(), V.get(), S.get(), b.get(), x.get());
    }
}
