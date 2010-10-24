#ifndef ASI_HYPER_MATRIX_H
#define ASI_HYPER_MATRIX_H

#include <asml/utils.h>
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
		size_t size(size_t dim) const;
		size_t dim() const;

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
		const MatrixPtr & mat();
		
		size_t rows() const;
		size_t cols() const;
		
		double operator()(const std::vector<size_t> & idxs1, const std::vector<size_t> & idxs2) const;
		double & operator()(const std::vector<size_t> & idxs1, const std::vector<size_t> & idxs2);
		
		const LinearHyperMatrix & dim1() const;
		const LinearHyperMatrix & dim2() const;

	private:
		const LinearHyperMatrix myDim1;
		const LinearHyperMatrix myDim2;
		
		MatrixPtr myMatrix;
	};
	
	// =====================================================================================================
	
	class HyperVector
	{
	public:
		HyperVector(const LinearHyperMatrix & dim);
		
		CVectorPtr vect() const;
		const VectorPtr & vect();
	
 		size_t size() const;
		
		double operator()(const std::vector<size_t> & idxs) const;
		double & operator()(const std::vector<size_t> & idxs);
		
		const LinearHyperMatrix & dim() const;

		void operator=(const VectorPtr & rhs);

	private:
		const LinearHyperMatrix myDim;
		
		VectorPtr myVector;
	};

	typedef boost::shared_ptr<HyperMatrix> HyperMatrixPtr;
	typedef boost::shared_ptr<HyperVector> HyperVectorPtr;

}

#endif
