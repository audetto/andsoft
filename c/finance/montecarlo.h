#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "matrix.h"

#include <vector>

namespace AndSoft {

  class MonteCarlo {
  public:
    MonteCarlo(unsigned int dim);

    void addVector(const std::vector<double> &val);
    double getMean(unsigned int dim) const;
    double getVar(unsigned int dim) const;
    double getCov(unsigned int i, unsigned int j) const;
    double getCorr(unsigned int i, unsigned int j) const;
    
    // to be used for control variates:
    // i product
    // j control variate
    double getBeta(unsigned int i, unsigned int j) const;

  private:
    const unsigned int myDim;
    std::vector<double> myMeans;
    Matrix myCovs;
    unsigned int myCount;
  };

}

#endif
