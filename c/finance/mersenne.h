#ifndef MERSENNE_H
#define MERSENNE_H

namespace AndSoft {
  
  class MersenneTwister {
    
  public:
    MersenneTwister(unsigned long s);
    MersenneTwister(const MersenneTwister& rhs);
    ~MersenneTwister();
    
    double genrand_real3();
    double normal_deviate();
    
  private:
    
    void init_genrand(unsigned long s);
    void init_by_array(unsigned long init_key[], int key_length);
    unsigned long genrand_int32();
    long genrand_int31();
    double genrand_real1();
    double genrand_real2();
    double genrand_res53();
    
    // static unsigned long mt[N]; /* the array for the state vector  */
    // static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */
    
    unsigned long *mt;
    int mti;
  };
  
}

#endif
