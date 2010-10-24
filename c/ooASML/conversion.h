#ifndef ASI_CONVERSION_H
#define ASI_CONVERSION_H

#include <asml/utils.h>

#include <com/sun/star/uno/Sequence.hxx>
#include <vector>

namespace ASI
{
    using namespace ::com::sun::star::uno;
    
    template <typename T, typename S>
	void ooConvert(const T & t, S & s);

    template <typename S, typename T>
	S ooDirectConvert(const T & t)
    {
	S s;
	ooConvert(t, s);
	return s;
    }

    void appendStdVectorToOOArgument(Sequence<Sequence<double> > & seq, const std::vector<double> & vect);
}

#endif
