/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef ASI_UTILS_H
#define ASI_UTILS_H

#include <asml/utils/error.h>
#include <boost/shared_ptr.hpp>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include <complex>
#include <string>
#include <map>

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

    void handler (const char * reason, const char * file, int line, int gsl_errno);

    typedef std::complex<double> cpl;

    extern const cpl I;

    inline int isPowerOfTwo(size_t x)
    {
        return !((x - 1) & x);
    }

    template<typename K, typename V>
    const V & findInMap(const std::map<K, V> & m, const K & k)
    {
        const typename std::map<K, V>::const_iterator it = m.find(k);
        if (it == m.end())
        {
            THROW_EXCEPTION("Cannot find value for " << k);
        }

        const V & v = it->second;
        return v;
    }

    template<typename T>
    std::pair<T, T> makeCanonical(const T & first, const T & second)
    {
        if (first < second)
            return std::make_pair(first, second);
        else
            return std::make_pair(second, first);
    }
}

#endif
