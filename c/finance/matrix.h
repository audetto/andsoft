#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include <gsl/gsl_matrix.h>
#include <boost/shared_ptr.hpp>

#include "utils.h"

namespace AndSoft {

  class Matrix {
  public:
    Matrix(unsigned int rows, unsigned int cols);
    Matrix(const gsl_matrix * gsl);

    ~Matrix();

    Matrix& operator=(const double rhs);

    const double operator()(unsigned int row, unsigned int col) const;
    double& operator()(unsigned int row, unsigned int col);

    unsigned int rows() const;
    unsigned int cols() const;

    const gsl_matrix* gsl() const;
    gsl_matrix* gsl();

    boost::shared_ptr<Matrix> choleskyDecomposition() const;
    boost::shared_ptr<Array> postMultiply(const Array& v) const;

  private:
    gsl_matrix* myData;
  };

}

#endif
