#include <asi/generator.h>

#include <assert.h>

namespace ASI
{

	using namespace std;

	HyperMatrixPtr generate(const QuantLib::StochasticProcess & process, 
							const vector<double> & lower, 
							const vector<double> & upper, 
							const vector<size_t> & sizes )
	{
		const size_t dim = process.size();
		
		assert(dim == lower.size());
		assert(dim == upper.size());
		assert(dim == sizes.size());
		
		vector<double> dx(dim);
		for (size_t i = 0; i < dim; ++i)
		{
			dx[i] = (upper[i] - lower[i]) / (sizes[i] - 1);
		}
		
		LinearHyperMatrix linearDims(sizes);
		
		HyperMatrixPtr mat(new HyperMatrix(linearDims, linearDims));
		
		vector<size_t> point(dim, 0);
		vector<size_t> temp(dim);
		QuantLib::Array location(dim);

		// we assume homogeneous model!
		// no support for time dependent coefficients
		QuantLib::Time zero = 0.0;
		
		do
		{
			
			if (linearDims.isBoundary(point))
			{
				// for the time being, boundary are absorbing
				(*mat)(point, point) = 0.0;
			}
			else
			{
				
				for (size_t i = 0; i < dim; ++i)
				{
					location[i] = lower[i] + dx[i] * point[i];
				}
				
				QuantLib::Disposable< QuantLib::Array > drift = process.drift (zero, location);
				QuantLib::Disposable< QuantLib::Matrix > diffusion = process.diffusion (zero, location);
				
				temp = point;
				
				for (size_t i = 0; i < dim; ++i)
				{
					const size_t base_i = temp[i];
					
					const double driftCoeff = drift[i] / (2.0 * dx[i]);
					const double diffusionCoeff = 0.5 * diffusion[i][i] / (dx[i] * dx[i]);
					

					// CENTRAL FIRST DERIVATIVE
					temp[i] = base_i + 1;
					(*mat)(point, temp) +=  driftCoeff;
					
					temp[i] = base_i - 1;
					(*mat)(point, temp) += -driftCoeff;


					// CENTRAL SECOND DERIVATIVE
					temp[i] = base_i + 1;
					(*mat)(point, temp) +=        diffusionCoeff;
					
					temp[i] = base_i;
					(*mat)(point, temp) += -2.0 * diffusionCoeff;
					
					temp[i] = base_i - 1;
					(*mat)(point, temp) +=        diffusionCoeff;
					
					
					for (size_t j = 0; j < i; ++j)
					{
						const size_t base_j = temp[j];
						
						// cross terms (2.0 * 0.5 * ....)
						const double crossCoeff = diffusion[i][j] / (4.0 * dx[i] * dx[j]);


						// CENTRAL CROSS DERIVATIVE
						temp[i] = base_i + 1;
						temp[j] = base_j + 1;
						(*mat)(point, temp) +=  crossCoeff;
						
						temp[i] = base_i - 1;
						temp[j] = base_j + 1;
						(*mat)(point, temp) += -crossCoeff;
						
						temp[i] = base_i + 1;
						temp[j] = base_j - 1;
						(*mat)(point, temp) += -crossCoeff;
						
						temp[i] = base_i - 1;
						temp[j] = base_j - 1;
						(*mat)(point, temp) +=  crossCoeff;
						

						// clean to reuse temp
						temp[j] = base_j;
					}
					
					// clean to reuse temp
					temp[i] = base_i;
				}
				
			}
			
		}
		while (linearDims.next(point));
		
		return mat;
		
	}
	
	bool checkMatrix(const HyperMatrix & hm, double & maxTime)
	{
		const gsl_matrix * mat = hm.mat().get();
		
		const size_t rows = mat->size1;
		const size_t cols = mat->size2;
		
		assert(rows == cols);
		
		double minDiag = 0.0;
		
		for (size_t i = 0; i < rows; ++i)
		{
			minDiag = min(minDiag, gsl_matrix_get(mat, i, i));
			for (size_t j = 0; j < cols; ++j)
			{
				if (j != i)
				{
					if (gsl_matrix_get(mat, i, j) < 0.0)
						return false;
				}
			}
		}
		
		maxTime = -1.0 / minDiag;
		
		return true;
	}
	
	
}
