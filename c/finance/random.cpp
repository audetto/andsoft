#include "random.h"
#include <gsl/gsl_cdf.h>

namespace AndSoft {

  RandomNumber::RandomNumber(const unsigned int dim) : myDim(dim) {
    myUniform.resize(dim);
    myNormal.resize(dim);
  }

  void RandomNumber::nextNormal() {
    nextUniform();
    for (unsigned int i = 0; i < myDim; ++i)
      myNormal[i] = gsl_cdf_ugaussian_Pinv(myUniform[i]);
  }

}
