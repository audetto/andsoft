#ifndef ASI_HYPER_MATRIX_H
#define ASI_HYPER_MATRIX_H

#include <asi/utils.h>
#include <vector>

namespace ASI
{
	// =====================================================================================================
	
	class LinearHyperMatrix
	{
	public:
		LinearHyperMatrix(const std::vector<size_t> & sizes);
		
		size_t coord2pos(const std::vector<size_t> & idxs) const;
		void pos2coord(const size_t pos, std::vector<size_t> & idxs) const;
		
		bool next(std::vector<size_t> & idxs) const;
		
		size_t size() const;
		
		bool isBoundary(const std::vector<size_t> & idxs) const;
		
	private:
		const std::vector<size_t> mySizes;
		const size_t myDim;
		const size_t myTotal;
	};
	
	// =====================================================================================================
	
	class HyperMatrix
	{
	public:
		HyperMatrix(const LinearHyperMatrix & dim1, const LinearHyperMatrix & dim2);
		
		CMatrixPtr mat() const;
		MatrixPtr mat();
		
		size_t rows() const;
		size_t cols() const;
		
		double operator()(const std::vector<size_t> & idxs1, const std::vector<size_t> & idxs2) const;
		double & operator()(const std::vector<size_t> & idxs1, const std::vector<size_t> & idxs2);
		
	private:
		const LinearHyperMatrix myDim1;
		const LinearHyperMatrix myDim2;
		
		MatrixPtr myMatrix;
	};
	
	typedef boost::shared_ptr<HyperMatrix> HyperMatrixPtr;

}

#endif
