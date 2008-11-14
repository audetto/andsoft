#ifndef ASI_UTILS_H
#define ASI_UTILS_H

#include <boost/shared_ptr.hpp>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include <complex>
#include <string>

namespace ASI
{

    struct MatrixDeleter
    {
	void operator()(gsl_matrix * matrix) const;
    };

    typedef boost::shared_ptr<gsl_matrix> MatrixPtr;
    typedef boost::shared_ptr<const gsl_matrix> CMatrixPtr;

    struct VectorDeleter
    {
	void operator()(gsl_vector * vector) const;
    };

    typedef boost::shared_ptr<gsl_vector> VectorPtr;
    typedef boost::shared_ptr<const gsl_vector> CVectorPtr;

    void __attribute__ ((__noreturn__)) error(const std::string & msg);

    void handler (const char * reason, const char * file, int line, int gsl_errno);

    typedef std::complex<double> cpl;

    extern const cpl I;

    inline int isPowerOfTwo(size_t x)
    {
	return !((x - 1) & x);
    }

}

#endif
