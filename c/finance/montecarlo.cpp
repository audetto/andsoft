#include "montecarlo.h"
#include "utils.h"

#include <math.h>

using namespace std;

namespace AndSoft {

  MonteCarlo::MonteCarlo(unsigned int dim) : myDim(dim), myMeans(dim, 0.0), myCovs(dim, dim), myCount(0) {
  }

  void MonteCarlo::addVector(const vector<double> &val) {
    myCount++;
    for (unsigned int i = 0; i < myDim; ++i)
      myMeans[i] += val[i];

    for (unsigned int i = 0; i < myDim; ++i) {
      const double first = val[i];
      for (unsigned int j = 0; j < i; ++j) {
	myCovs(i, j) += first * val[j];
	myCovs(j, i) = myCovs(i, j);
      }
      myCovs(i, i) += SQR(first);
    }
  }

  double MonteCarlo::getMean(unsigned int dim) const {
    return myMeans[dim] / myCount;
  }

  double MonteCarlo::getVar(unsigned int dim) const {
    const double mom2 = myCovs(dim, dim) / myCount;
    const double mean = getMean(dim);

    return mom2 - mean * mean;
  }

  double MonteCarlo::getCov(unsigned int i, unsigned int j) const {
    const double mix_mom = myCovs(i, j) / myCount;
    const double mean_i = getMean(i);
    const double mean_j = getMean(j);

    return mix_mom - mean_i * mean_j;
  }

  double MonteCarlo::getCorr(unsigned int i, unsigned int j) const {
    const double cov   = getCov(i, j);
    const double var_i = getVar(i);
    const double var_j = getVar(j);

    return cov / sqrt(var_i * var_j);
  }

  double MonteCarlo::getBeta(unsigned int i, unsigned int j) const {
    const double beta = getCov(i, j) / getVar(j);
    return beta;
  }
  
}
