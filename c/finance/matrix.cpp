#include "matrix.h"
#include "utils.h"

using namespace std;

namespace AndSoft {

  Matrix::Matrix(unsigned int rows, unsigned int cols) {
    gsl_matrix_calloc(rows, cols);
  }

  Matrix::Matrix(const gsl_matrix * gsl) {
    myData = gsl_matrix_alloc(gsl->size1, gsl->size2);
    gsl_matrix_memcpy(myData, gsl);
  }

  Matrix::~Matrix() {
    gsl_matrix_free(myData);
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

  const gsl_matrix* Matrix::gsl_const() const {
    return myData;
  }

  gsl_matrix* Matrix::gsl() {
    return myData;
  }

}
