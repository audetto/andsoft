#include <asi/fastexp.h>

#include <assert.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>

#include <iostream>


using namespace std;

ostream & operator<<(ostream & out, const gsl_matrix * mat)
{
    for (size_t i = 0; i < mat->size1; ++i)
    {
	out << "[" << i << ", " << mat->size2 << "]: ";
	for (size_t j = 0; j < mat->size2; ++j)
	    out << gsl_matrix_get(mat, i, j) << ", ";
	
	out << endl;
    }
    
    return out;
}

ostream & operator<<(ostream & out, const gsl_vector * vec)
{
    out << "[" << vec->size << "]: ";
    
    for (size_t i = 0; i < vec->size; ++i)
    {
	out << gsl_vector_get(vec, i) << ", ";
    }
    
    out << endl;
    
    return out;
}

ostream & operator<<(ostream & out, const gsl_matrix_complex * mat)
{
    for (size_t i = 0; i < mat->size1; ++i)
    {
	out << "[" << i << ", " << mat->size2 << "]: ";
	for (size_t j = 0; j < mat->size2; ++j)
	{
	    const gsl_complex z = gsl_matrix_complex_get(mat, i, j);
	    out << GSL_REAL(z) << "-" << GSL_IMAG(z) << ", ";
	}
	
	out << endl;
    }
    
    return out;
}

ostream & operator<<(ostream & out, const gsl_vector_complex * vec)
{
    out << "[" << vec->size << "]: ";
    
    for (size_t i = 0; i < vec->size; ++i)
    {
	const gsl_complex z = gsl_vector_complex_get(vec, i);
	out << GSL_REAL(z) << "-" << GSL_IMAG(z) << ", ";
    }
    
    out << endl;
    
    return out;
}

namespace ASI
{

    MatrixPtr expViaEigenvalues(const double time, const MatrixPtr & mat)
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
    
    MatrixPtr expViaGSL(const double time, const CMatrixPtr & mat)
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
    
    MatrixPtr expViaExplicit(const double time, const CMatrixPtr & mat, const size_t powerOfTwo)
    {
	const double steps = pow(2.0, powerOfTwo);
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

    MatrixPtr expViaTheta(const double time, const CMatrixPtr & mat, const size_t powerOfTwo, const double theta)
    {
	const double steps = pow(2.0, powerOfTwo);
	const double dt = time / steps;
	
	const size_t rows = mat->size1;
	const size_t cols = mat->size2;
	
	if (rows != cols)
	    error("Cannot exponentiate rectangular matrix.");
	
	MatrixPtr onePlusOneMinusThetaTA(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	MatrixPtr oneMinusThetaTA       (gsl_matrix_alloc(rows, cols), MatrixDeleter());
	MatrixPtr temp(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	
	for (size_t i = 0; i < rows; ++i)
	{
	    for (size_t j = 0; j < rows; ++j)
	    {
		const double r = gsl_matrix_get(mat.get(), i, j);
		const double one = (i == j) ? 1.0 : 0.0;
		gsl_matrix_set(onePlusOneMinusThetaTA.get(), i, j, one + (1.0 - theta) * dt * r);
		gsl_matrix_set(oneMinusThetaTA.get(),        i, j, one -        theta  * dt * r);
	    }
	}
	
	gsl_permutation * perm = gsl_permutation_alloc(rows);
	int s;
	gsl_linalg_LU_decomp(oneMinusThetaTA.get(), perm, &s);

	MatrixPtr cnMat(gsl_matrix_alloc(rows, cols), MatrixDeleter());
	for (size_t i = 0; i < rows; ++i)
	{
	    gsl_vector_const_view b = gsl_matrix_const_column(onePlusOneMinusThetaTA.get(), i);
	    gsl_vector_view x       = gsl_matrix_column(cnMat.get(), i);
	    gsl_linalg_LU_solve(oneMinusThetaTA.get(), perm, &b.vector, &x.vector);
	}
	gsl_permutation_free(perm);
	

	for (size_t i = 0; i < powerOfTwo; ++i)
	{
	    gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, cnMat.get(), cnMat.get(), 0.0, temp.get());
	    swap(temp, cnMat);
	}
	
	return cnMat;
    }

	VectorPtr expViaTraditional(const double time, const MatrixPtr & mat, const size_t powerOfTwo, const VectorPtr & rhs)
	{
		const size_t steps = 1 << powerOfTwo;
		const double dt = time / steps;
		
		const size_t rows = mat->size1;
		const size_t cols = mat->size2;
		
		if (rows != cols)
			error("Cannot exponentiate rectangular matrix.");
		
		MatrixPtr oneMinusTA(gsl_matrix_alloc(rows, cols), MatrixDeleter());
		
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < rows; ++j)
			{
				const double r = gsl_matrix_get(mat.get(), i, j);
				const double one = (i == j) ? 1.0 : 0.0;
				gsl_matrix_set(oneMinusTA.get(), i, j, one - dt * r);
			}
		}
		
		gsl_permutation * perm = gsl_permutation_alloc(rows);
		int s;
		gsl_linalg_LU_decomp(oneMinusTA.get(), perm, &s);

		VectorPtr x(gsl_vector_alloc(rows), VectorDeleter());
		VectorPtr b = rhs;

		for (size_t i = 0; i < steps; ++i)
		{
			gsl_linalg_LU_solve(oneMinusTA.get(), perm, b.get(), x.get());
			swap(b, x);
		}
		
		gsl_permutation_free(perm);

		return b;
	}
	
    void fastexp_try()
    {
	MatrixPtr A(gsl_matrix_alloc(2, 2), MatrixDeleter());
	gsl_matrix_set(A.get(), 0, 0, -1.0);
	gsl_matrix_set(A.get(), 0, 1, 5.0);
	gsl_matrix_set(A.get(), 1, 0, 0.0);
	gsl_matrix_set(A.get(), 1, 1, -1.1);

	double t = 1.0;

	{
	    MatrixPtr res = expViaEigenvalues(t, A);
	    cout << "Result" << endl;
	    cout << res;
	}
	{
	    MatrixPtr res = expViaGSL(t, A);
	    cout << "Result" << endl;
	    cout << res;
	}
	{
	    MatrixPtr res = expViaTheta(t, A, 45, 0.5);
	    cout << "Result" << endl;
	    cout << res;
	}
    }

}
