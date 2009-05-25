#include <asi/projection.h>
#include <asi/heston.h>
#include <asi/numerics.h>
#include <asi/fastexp.h>
#include <asi/pde.h>

using namespace ASI;

int main()
{

    try
    {
		MatrixPtr a(gsl_matrix_calloc(5, 2), MatrixDeleter());
		VectorPtr b(gsl_vector_calloc(5), VectorDeleter());
		VectorPtr x(gsl_vector_calloc(2), VectorDeleter());
		
		gsl_matrix_set(a.get(), 0, 0,  -1.0);
		gsl_matrix_set(a.get(), 0, 1,  -1.0);
		gsl_vector_set(b.get(), 0,    -10.0);
		
		gsl_matrix_set(a.get(), 1, 0,  -1.0);
		gsl_matrix_set(a.get(), 1, 1,   0.0);
		gsl_vector_set(b.get(), 1,     -3.0);
		
		gsl_matrix_set(a.get(), 2, 0,   1.0);
		gsl_matrix_set(a.get(), 2, 1,   0.0);
		gsl_vector_set(b.get(), 2,     10.0);
		
		gsl_matrix_set(a.get(), 3, 0,   0.0);
		gsl_matrix_set(a.get(), 3, 1,  -1.0);
		gsl_vector_set(b.get(), 3,      0.0);
		
		gsl_matrix_set(a.get(), 4, 0,   0.0);
		gsl_matrix_set(a.get(), 4, 1,   1.0);
		gsl_vector_set(b.get(), 4,      4.0);
		
		gsl_vector_set(x.get(), 0,    -49.0);
		gsl_vector_set(x.get(), 1,     50.0);
		/*
		  MatrixPtr a(gsl_matrix_calloc(2, 2), MatrixDeleter());
		  VectorPtr b(gsl_vector_calloc(2), VectorDeleter());
		  VectorPtr x(gsl_vector_calloc(2), VectorDeleter());
		  
		  gsl_matrix_set(a.get(), 0, 0,  1.0);
		  gsl_matrix_set(a.get(), 0, 1,  0.0);
		  gsl_vector_set(b.get(), 0,     0.0);
		  
		  gsl_matrix_set(a.get(), 1, 0,  0.0);
		  gsl_matrix_set(a.get(), 1, 1,  1.0);
		  gsl_vector_set(b.get(), 1,     0.0);
		  
		  gsl_vector_set(x.get(), 0,    10.0);
		  gsl_vector_set(x.get(), 1,    50.0);*/
		
		
		const size_t iter = 100;
		const double tol = 1.0e-10;
		dykstraProjection(x.get(), a.get(), b.get(), iter, tol);
		
		//	heston_try();
		
		//	numerics_try();
		
		// fastexp_try();

		pde_try();
		
    }
    catch (const std::exception & e)
    {
		printf("%s\n", e.what());
    }
    return 0;
}
