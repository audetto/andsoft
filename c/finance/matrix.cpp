#include "matrix.h"
#include "utils.h"

#include <gsl/gsl_linalg.h>

using namespace std;

namespace AndSoft {

  Matrix::Matrix(unsigned int rows, unsigned int cols) {
    myData = gsl_matrix_calloc(rows, cols);
  }

  Matrix::Matrix(const gsl_matrix * gsl) {
    myData = gsl_matrix_alloc(gsl->size1, gsl->size2);
    gsl_matrix_memcpy(myData, gsl);
  }

  Matrix::~Matrix() {
    gsl_matrix_free(myData);
  }

  Matrix& Matrix::operator=(const double rhs) {
    gsl_matrix_set_all(myData, rhs);
    return *this;
  }

  unsigned int Matrix::rows() const {
    return myData->size1;
  }
    
  unsigned int Matrix::cols() const {
    return myData->size2;
  }

  const double Matrix::operator()(unsigned int row, unsigned int col) const {
    return gsl_matrix_get(myData, row, col);
  }

  double& Matrix::operator()(unsigned int row, unsigned int col) {
    if (row < myData->size1 && col < myData->size2)
      return myData->data[row * myData->tda + col];
    else
      runtime_error("Matrix::operator() Out of Bounds!");
  }

  const gsl_matrix* Matrix::gsl() const {
    return myData;
  }

  gsl_matrix* Matrix::gsl() {
    return myData;
  }

  boost::shared_ptr<Matrix> Matrix::choleskyDecomposition() const {
    boost::shared_ptr<Matrix> copy(new Matrix(*this));
    check_gsl_error(gsl_linalg_cholesky_decomp(copy->gsl()));
    return copy;
  }

  boost::shared_ptr<Array> Matrix::postMultiply(const Array& v) const {
    const unsigned int n = v.size();
    if (myData->size2 != n)
      runtime_error("Matrix::postMultiply invalid sizes!");
    boost::shared_ptr<Array> result(new Array(myData->size1));

    for (unsigned int i = 0; i < myData->size1; ++i) {
      double temp = 0.0;
      for (unsigned int j = 0; j < n; ++j)
	temp += gsl_matrix_get(myData, i, j) * v[j];
      (*result)[i] = temp;
    }

    return result;
  }

}
