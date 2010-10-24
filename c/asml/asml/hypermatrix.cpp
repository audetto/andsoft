#include <asml/hypermatrix.h>

#include <numeric>
#include <functional>
#include <stdlib.h>
#include <assert.h>

namespace ASI
{

	using namespace std;
	
	// =====================================================================================================
	
	LinearHyperMatrix::LinearHyperMatrix(const vector<size_t> & sizes) :
		mySizes(sizes), 
		myDim(mySizes.size()), 
		myTotal(accumulate(mySizes.begin(), mySizes.end(), 1, multiplies<size_t>()))
	{
	}
	
	size_t LinearHyperMatrix::coord2pos(const vector<size_t> & idxs) const
	{
		assert(idxs.size() == myDim);
		
		size_t pos = 0;
		size_t cum = 1;
		for (size_t i = 0; i < myDim; ++i)
		{
			assert(idxs[i] < mySizes[i]);
			pos += idxs[i] * cum;
			cum *= mySizes[i];
		}
		
		return pos;
	}
	
	void LinearHyperMatrix::pos2coord(size_t pos, vector<size_t> & idxs) const
	{
		assert(pos < myTotal);
		
		idxs.resize(myDim);
		for (size_t i = 0; i < myDim; ++i)
		{
			const div_t res = div(int(pos), int(mySizes[i]));
			idxs[i] = res.rem;
			pos = res.quot;
		}    
	}
	
	bool LinearHyperMatrix::next(vector<size_t> & idxs) const
	{
		for (size_t i = 0; i < myDim; ++i)
		{
			++idxs[i];
			if (idxs[i] < mySizes[i])
			{
				return true;
			}
			else
			{
				idxs[i] = 0;
			}
		}
		
		return false;
	}
	
	size_t LinearHyperMatrix::size() const
	{
		return myTotal;
	}

	size_t LinearHyperMatrix::size(size_t dim) const
	{
		return mySizes[dim];
	}

	size_t LinearHyperMatrix::dim() const
	{
		return myDim;
	}
	
	bool LinearHyperMatrix::isBoundary(const vector<size_t> & idxs) const
	{
		for (size_t i = 0; i < myDim; ++i)
		{
			if (idxs[i] == 0 || idxs[i] == mySizes[i] - 1)
			{
				return true;
			}
		}
		
		return false;
	}
	
	
	// =====================================================================================================
	
	HyperMatrix::HyperMatrix(const LinearHyperMatrix & dim1, const LinearHyperMatrix & dim2) :
		myDim1(dim1), myDim2(dim2), myMatrix(gsl_matrix_calloc(myDim1.size(), myDim2.size()), MatrixDeleter())
	{
	}
	
	double HyperMatrix::operator()(const vector<size_t> & idxs1, const vector<size_t> & idxs2) const
	{
		const size_t pos1 = myDim1.coord2pos(idxs1);
		const size_t pos2 = myDim2.coord2pos(idxs2);
		return gsl_matrix_get(myMatrix.get(), pos1, pos2);
	}
	
	double & HyperMatrix::operator()(const vector<size_t> & idxs1, const vector<size_t> & idxs2)
	{
		const size_t pos1 = myDim1.coord2pos(idxs1);
		const size_t pos2 = myDim2.coord2pos(idxs2);
		double * ptr = gsl_matrix_ptr(myMatrix.get(), pos1, pos2);
		
		assert(ptr);
		
		return *ptr;
	}
	
	CMatrixPtr HyperMatrix::mat() const
	{
		return myMatrix;
	}
	
	const MatrixPtr & HyperMatrix::mat()
	{
		return myMatrix;
	}
	
	size_t HyperMatrix::rows() const
	{
		return myDim1.size();
	}
	
	size_t HyperMatrix::cols() const
	{
		return myDim2.size();
	}

	const LinearHyperMatrix & HyperMatrix::dim1() const
	{
		return myDim1;
	}

	const LinearHyperMatrix & HyperMatrix::dim2() const
	{
		return myDim2;
	}
		
	// =====================================================================================================
	
	HyperVector::HyperVector(const LinearHyperMatrix & dim) :
		myDim(dim), myVector(gsl_vector_calloc(myDim.size()), VectorDeleter())
	{
	}
	
	double HyperVector::operator()(const vector<size_t> & idxs) const
	{
		const size_t pos = myDim.coord2pos(idxs);
		return gsl_vector_get(myVector.get(), pos);
	}
	
	double & HyperVector::operator()(const vector<size_t> & idxs)
	{
		const size_t pos = myDim.coord2pos(idxs);
		double * ptr = gsl_vector_ptr(myVector.get(), pos);
		
		assert(ptr);
		
		return *ptr;
	}
	
	CVectorPtr HyperVector::vect() const
	{
		return myVector;
	}
	
	const VectorPtr & HyperVector::vect()
	{
		return myVector;
	}
	
	size_t HyperVector::size() const
	{
		return myDim.size();
	}
	
	const LinearHyperMatrix & HyperVector::dim() const
	{
		return myDim;
	}

	void HyperVector::operator=(const VectorPtr & rhs)
	{
		if (rhs.get() != myVector.get())
		{
			assert(myVector->size == rhs->size);
			myVector = rhs;
		}
	}


}
