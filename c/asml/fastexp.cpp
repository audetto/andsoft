#include <asi/fastexp.h>

#include <assert.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>

namespace ASI
{
    MatrixPtr expViaEigenvalues(const double time, MatrixPtr & mat)
    {
	const size_t rows = mat->size1;
	const size_t cols = mat->size2;
	
	if (rows != cols)
	    error("Cannot exponentiate rectangular matrix.");
	
	gsl_eigen_nonsymmv_workspace * workspace = gsl_eigen_nonsymmv_alloc (rows);
	
	gsl_vector_complex * eval = gsl_vector_complex_alloc (rows);
	gsl_matrix_complex * evec = gsl_matrix_complex_alloc (rows, rows);
	
	const int ok = gsl_eigen_nonsymmv (mat.get(), eval, evec, workspace);
	
	gsl_eigen_nonsymmv_free (workspace);
	
	if (ok == 0)
	{
	    gsl_matrix_complex * diagExp = gsl_matrix_complex_calloc (rows, rows);
	    
	    for (size_t i = 0; i < rows; ++i)
	    {
		const gsl_complex z     = gsl_vector_complex_get(eval, i);
		const gsl_complex tz    = gsl_complex_mul_real  (z, time);
		const gsl_complex exptz = gsl_complex_exp       (tz);
		
		gsl_matrix_complex_set(diagExp, i, i, exptz);
	    }
	    
	    gsl_matrix_complex * temp = gsl_matrix_complex_calloc (rows, rows);
	    
	    gsl_complex one;
	    GSL_SET_COMPLEX(&one, 1.0, 0.0);
	    
	    gsl_complex zero;
	    GSL_SET_COMPLEX(&zero, 0.0, 0.0);
	    
	    gsl_blas_zgemm (CblasNoTrans, CblasNoTrans, one, evec, diagExp, zero, temp);
	    
	    gsl_permutation * perm = gsl_permutation_alloc(rows);
	    int s;
	    gsl_linalg_complex_LU_decomp(evec, perm, &s);
	    gsl_linalg_complex_LU_invert(evec, perm, diagExp);
	    
	    gsl_blas_zgemm (CblasNoTrans, CblasNoTrans, one, temp, diagExp, zero, evec);
	    
	    gsl_permutation_free(perm);
	    
	    MatrixPtr result(gsl_matrix_alloc(rows, rows), MatrixDeleter());
	    
	    for (size_t i = 0; i < rows; ++i)
	    {
		for (size_t j = 0; j < rows; ++j)
		{
		    const gsl_complex z = gsl_matrix_complex_get(evec, i, j);
		    const double r = GSL_REAL(z);
		    gsl_matrix_set(result.get(), i, j, fabs(r) < DBL_EPSILON ? 0.0 : r);
		}
	    }
	    
	    gsl_matrix_complex_free(diagExp);
	    gsl_matrix_complex_free(temp);
	    
	    return result;
	}
	
	gsl_vector_complex_free(eval);
	gsl_matrix_complex_free(evec);
	
	error("Cannot compute eigenvalues");
    }
    
    MatrixPtr expViaGSL(const double time, const MatrixPtr & mat)
    {
	const size_t rows = mat->size1;
	const size_t cols = mat->size2;

	if (rows != cols)
	    error("Cannot exponentiate rectangular matrix.");

	gsl_matrix * tA = gsl_matrix_alloc(mat->size1, mat->size2);
	gsl_matrix_memcpy (tA, mat.get());
	gsl_matrix_scale (tA, time);
	
	MatrixPtr exptA(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	
	const gsl_mode_t mode = GSL_PREC_DOUBLE;
	gsl_linalg_exponential_ss( tA, exptA.get(), mode );
	
	gsl_matrix_free(tA);
	
	return exptA;
    }
    
    MatrixPtr expViaExplicit(const double time, const MatrixPtr & mat, const size_t powerOfTwo)
    {
	const size_t steps = 1 << powerOfTwo;
	const double dt = time / steps;
	
	const size_t rows = mat->size1;
	const size_t cols = mat->size2;
	
	if (rows != cols)
	    error("Cannot exponentiate rectangular matrix.");
	
	MatrixPtr onePlustA(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	MatrixPtr temp(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	
	for (size_t i = 0; i < rows; ++i)
	{
	    for (size_t j = 0; j < rows; ++j)
	    {
		const double r = gsl_matrix_get(mat.get(), i, j);
		const double one = (i == j) ? 1.0 : 0.0;
		gsl_matrix_set(onePlustA.get(), i, j, one + dt * r);
	    }
	}
	
	for (size_t i = 0; i < powerOfTwo; ++i)
	{
	    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, onePlustA.get(), onePlustA.get(), 0.0, temp.get());
	    swap(temp, onePlustA);
	}
	
	return onePlustA;
    }

}
