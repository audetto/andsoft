/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/utils/error.h>
#include "ConversionOut.h"

using namespace ::com::sun::star::uno;
using namespace ::rtl;

namespace ASI
{
    void ooConvertOut(const std::string & str, OUString & s1)
    {
        s1 = OUString::createFromAscii(str.c_str());
    }

    void ooConvertOut(const MatrixPtr & matPtr, Sequence<Sequence<double> > & mat)
    {
        const size_t rows = matPtr->size1;
        const size_t cols = matPtr->size2;
        
        mat = Sequence<Sequence<double> >(rows);
        
        for (size_t i = 0; i < rows; ++i)
        {
            mat[i].realloc(cols);
            for (size_t j = 0; j < cols; ++j)
            {
                mat[i][j] = gsl_matrix_get(matPtr.get(), i, j);
            }
        }
    }
    
    void ooConvertOut(const VectorPtr & vectPtr, Sequence<Sequence<double> >& vect)
    {
        const size_t rows = vectPtr->size;
        const size_t cols = 1;
        
        vect = Sequence<Sequence<double> > (rows);
        
        for (size_t i = 0; i < rows; ++i)
        {
            vect[i].realloc(cols);
            vect[i][0] = gsl_vector_get(vectPtr.get(), i);
        }
    }
    
    void appendStdVectorToOOArgument(Sequence<Sequence<double> > & seq, const std::vector<double> & vect)
    {
        const size_t rows = seq.getLength();
        const size_t vect_rows = vect.size();
        
        if (rows == 0)
            seq.realloc(vect_rows);
        else
            if (vect.size() != rows)
                error("size mismatch!");
        
        for (size_t i = 0; i < vect_rows; ++i)
        {
            const size_t cols = seq[i].getLength();
            seq[i].realloc(cols + 1);
            seq[i][cols] = vect[i];
        }
    }
    
    void ooConvertOut(const cpl & cplNum, OUString & cplStr)
    {
        const double real = cplNum.real();
        const double img  = cplNum.imag();
        
        std::ostringstream out;
        out << real << std::ios_base::showpos << img << "i";

        std::string str = out.str();
        ooConvertOut(str, cplStr);
    }
}
