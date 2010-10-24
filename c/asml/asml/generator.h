#ifndef ASI_GENERATOR_H
#define ASI_GENERATOR_H

#include <asml/hypermatrix.h>

#include <ql/stochasticprocess.hpp>


namespace ASI
{
	HyperMatrixPtr generate(const QuantLib::StochasticProcess & process, 
							const std::vector<double> & lower, 
							const std::vector<double> & upper, 
							const std::vector<size_t> & sizes );
	
	bool checkMatrix(const HyperMatrix & mat, double & maxTime);
}

#endif
