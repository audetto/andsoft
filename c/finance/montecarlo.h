#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "matrix.h"

#include <vector>

namespace AndSoft {

  class MonteCarlo {
  public:
    MonteCarlo(const unsigned int dim);

    void addVector(const std::vector<double> &val);
    double getMean(const unsigned int dim) const;
    double getVar(const unsigned int dim) const;
    double getCov(const unsigned int i, const unsigned int j) const;
    double getCorr(const unsigned int i, const unsigned int j) const;
    
    // to be used for control variates:
    // i product
    // j control variate
    double getBeta(const unsigned int i, const unsigned int j) const;

  private:
    const unsigned int myDim;
    std::vector<double> myMeans;
    Matrix myCovs;
    unsigned int myCount;
  };

}

#endif
