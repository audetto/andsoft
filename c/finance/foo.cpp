#include "mersenne.h"
#include "timer.h"
#include "montecarlo.h"

#include <boost/shared_ptr.hpp>
#include <iostream>

using namespace AndSoft;
using namespace std;

int main() {
  MersenneTwister mt(0);

  const unsigned int N = 1000000;
  const unsigned int DIM = 3;

  MonteCarlo mc(DIM);
  vector<double> v(DIM);

  const Timer timer;
  for (unsigned int i = 0; i < N; ++i) {
    for (unsigned int j = 0; j < DIM; ++j)
      v[j] = mt.normal_deviate();

    mc.addVector(v);
  }
  const double el = timer.elapsed();

  for (unsigned int i = 0; i < DIM; ++i) {
    cout << mc.getMean(i) << " <> ";
    for (unsigned int j = 0; j < DIM; ++j)
      cout << mc.getCov(i, j) << " = ";
      cout << endl;
  }
  
  cout << el << endl;
}
