#include "conversion.h"

namespace ASI
{
    MatrixPtr matrixFromOOArgument(const Sequence<Sequence<double> >& mat)
    {
	const size_t rows = mat.getLength();

	size_t cols = 0;

	for (size_t i = 0; i < rows; ++i)
	{
	    cols = std::max(cols, size_t(mat[i].getLength()));
	}

	MatrixPtr result(gsl_matrix_calloc(rows, cols), MatrixDeleter());

	for (size_t i = 0; i < rows; ++i)
	{
	    size_t thisCols = mat[i].getLength();
	    for (size_t j = 0; j < thisCols; ++j)
	    {
		gsl_matrix_set (result.get(), i, j, mat[i][j]);
	    }
	    
	}

	return result;
    }

    Sequence<Sequence<double> > matrixToOOArgument(const CMatrixPtr & mat)
    {
	const size_t rows = mat->size1;
	const size_t cols = mat->size2;

	Sequence<Sequence<double> > result(rows);
	
	for (size_t i = 0; i < rows; ++i)
	{
	    result[i].realloc(cols);
	    for (size_t j = 0; j < cols; ++j)
	    {
		result[i][j] = gsl_matrix_get(mat.get(), i, j);
	    }
	}

	return result;
	
    }

    VectorPtr vectorFromOOArgument(const Sequence<Sequence<double> >& vect)
    {
	CMatrixPtr matrix = matrixFromOOArgument(vect);

	if (matrix->size1 == 1)
	{
	    // row vector
	    const size_t len = matrix->size2;
	    VectorPtr result(gsl_vector_calloc(len), VectorDeleter());
	    gsl_matrix_get_row (result.get(), matrix.get(), 0);
	    return result;
	}
	else if (matrix->size2 == 1)
	{
	    // column vector
	    const size_t len = matrix->size1;
	    VectorPtr result(gsl_vector_calloc(len), VectorDeleter());
	    gsl_matrix_get_col (result.get(), matrix.get(), 0);
	    return result;
	}
	else
	{
	    error("Not a vector");
	}
    }

    Sequence<Sequence<double> > vectorToOOArgument(const CVectorPtr & vect)
    {
	const size_t rows = vect->size;
	const size_t cols = 1;

	Sequence<Sequence<double> > result(rows);
	
	for (size_t i = 0; i < rows; ++i)
	{
	    result[i].realloc(cols);
	    result[i][0] = gsl_vector_get(vect.get(), i);
	}

	return result;
    }

    std::vector<double> stdVectorFromOOArgument(const Sequence<Sequence<double> >& vect)
    {
	CVectorPtr vector = vectorFromOOArgument(vect);

	const size_t dim = vector->size;

	std::vector<double> res(dim);

	for (size_t i = 0; i < dim; ++i)
	{
	    res[i] = gsl_vector_get(vector.get(), i);
	}

	return res;
    }

    Sequence<Sequence<double> > stdVectorToOOArgument(const std::vector<double> & vect)
    {
	const size_t rows = vect.size();
	const size_t cols = 1;

	Sequence<Sequence<double> > result(rows);
	
	for (size_t i = 0; i < rows; ++i)
	{
	    result[i].realloc(cols);
	    result[i][0] = vect[i];
	}

	return result;
    }
}
