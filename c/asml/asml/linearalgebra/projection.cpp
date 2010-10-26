#include <asml/linearalgebra/projection.h>
#include <asml/utils/error.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>

#include <vector>

using namespace std;

namespace ASI
{
    void projectOnSubspace(gsl_vector * x, const gsl_matrix * a, const gsl_vector * b)
    {
	const size_t rows = x->size;
	const size_t cols = b->size;

	if (a->size1 != rows || a->size2 != cols)
	    error("Bad size");

	VectorPtr b2(gsl_vector_alloc(cols), VectorDeleter());
	gsl_vector_memcpy(b2.get(), b);

	// b2 = a'x - b
	gsl_blas_dgemv(CblasTrans, 1.0, a, x, -1.0, b2.get());

	MatrixPtr a2(gsl_matrix_calloc(cols, cols), MatrixDeleter());

	// a2 = a'a
	gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, a, a, 0.0, a2.get());

	int s;
	gsl_permutation * p = gsl_permutation_alloc (cols);
	gsl_linalg_LU_decomp (a2.get(), p, &s);

	// b2 = inv(a2) * b2 = (a'a)-1(a'x-b)
	gsl_linalg_LU_svx(a2.get(), p, b2.get());

	gsl_permutation_free (p);

	gsl_blas_dgemv(CblasNoTrans, -1.0, a, b2.get(), 1.0, x);
    }

    bool projectOnHalf1DSubspace(gsl_vector * x, const gsl_vector * a, const double b)
    {
	const size_t dim = x->size;

	if (a->size != dim)
	    error("Bad size");

	double num, den;
	gsl_blas_ddot (a, x, &num);
	num -= b;

	const double changed = num > 0.0;

	if (changed)
	{
	    gsl_blas_ddot (a, a, &den);
	    gsl_blas_daxpy (-num / den, a, x);
	}
	
	return changed;
    }

    void dykstraProjection(gsl_vector * x, const gsl_matrix * a, const gsl_vector * b, const size_t iter, const double tol)
    {
	const size_t cols = x->size; // dimension
	const size_t rows = b->size; // number of projections

	if (a->size1 != rows || a->size2 != cols)
	    error("Bad size");

	vector<VectorPtr> y(rows);
	for (size_t i = 0; i < rows; ++i)
	{
	    y[i].reset(gsl_vector_calloc(cols), VectorDeleter());
	}

	VectorPtr x_prec(gsl_vector_alloc(cols), VectorDeleter());

	for (size_t k = 0; k < iter; ++k)
	{
	    double sumOfNorms = 0.0;
	    for (size_t i = 0; i < rows; ++i)
	    {
		gsl_vector_memcpy(x_prec.get(), x);
		gsl_vector_sub (x, y[i].get());

		gsl_vector_const_view thisRow = gsl_matrix_const_row (a, i);

		projectOnHalf1DSubspace(x, &thisRow.vector, gsl_vector_get(b, i));

		gsl_vector_sub(x_prec.get(), x);  // x_prec = x(i-1) - x(i)

		double norm2;
		gsl_blas_ddot (x_prec.get(), x_prec.get(), &norm2);
		sumOfNorms += norm2;

		gsl_vector_sub (y[i].get(), x_prec.get());
	    }
	    if (sumOfNorms < tol)
		return;
	}
	// no convergence. return anyway.
	// was problem feasible?
    }

}
