#ifndef RANDOM_H
#define RANDOM_H

#include "utils.h"

namespace AndSoft {

  class RandomNumber {  
  public:

    virtual ~RandomNumber() { }

    const Array& uniform() const {
      return myUniform;
    }
    const Array& normal() const {
      return myNormal;
    }

    virtual void nextUniform() = 0;
    void nextNormal();

  protected:
    RandomNumber(const unsigned int dim);
    
    const unsigned int myDim;
    Array myUniform;
    Array myNormal;
  
  };

}

#endif
