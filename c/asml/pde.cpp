#include <asi/pde.h>

#include <asi/generator.h>
#include <asi/fastexp.h>
#include <asi/implied.h>
#include <asi/brownianmotionwithdrift.h>
#include <functional>

#include <gsl/gsl_blas.h>

namespace ASI
{

	class Payoff : public std::unary_function<const std::vector<double> &, double>
	{
	public:
		~Payoff() {}
		virtual double operator()(const std::vector<double> & ) const = 0;
	};

	class PDE
	{
	public:

		class Solution
		{
		public:
			Solution & operator+=(double val);
			Solution & operator+=(const Payoff & val);

			Solution & solveTo(double t);
			Solution & solveTo2(double t);

			double time() const;

			const LinearHyperMatrix & dim() const;
			double value(const std::vector<size_t> & point) const;

		private:
			Solution(const PDE & pde, const LinearHyperMatrix & dim, double time);

			const PDE      & myPde;
			HyperVectorPtr   myVector;
			double           myTime;

			friend class PDE;
		};


		PDE(const QuantLib::StochasticProcess & process, 
			const std::vector<double> & lower, 
			const std::vector<double> & upper, 
			const std::vector<size_t> & sizes);

		Solution solve(double time) const;

		void location(const std::vector<size_t> & point, std::vector<double> & loc) const;

	private:
		const QuantLib::StochasticProcess & myProcess;
		const size_t                        myDimension;
		const std::vector<double>           myLower;
		const std::vector<double>           myUpper;
		const std::vector<size_t>           mySizes;

		std::vector<double>                 myDx;

		HyperMatrixPtr                      myGenerator;

		double                              myMaxDT;
	};

		
	PDE::PDE(const QuantLib::StochasticProcess & process, 
			 const std::vector<double> & lower, 
			 const std::vector<double> & upper, 
			 const std::vector<size_t> & sizes) : myProcess(process), myDimension(myProcess.size()), myLower(lower), myUpper(upper), mySizes(sizes)
	{
		myGenerator = generate(process, lower, upper, sizes);

		assert(checkMatrix(*myGenerator, myMaxDT));

		myDx.resize(myDimension);
		for (size_t i = 0; i < myDimension; ++i)
		{
			myDx[i] = (myUpper[i] - myLower[i]) / (mySizes[i] - 1);
		}
	}

	PDE::Solution PDE::solve(double time) const
	{
		return Solution(*this, myGenerator->dim1(), time);
	}

	void PDE::location(const std::vector<size_t> & point, std::vector<double> & loc) const
	{
		for (size_t i = 0; i < myDimension; ++i)
		{
			loc[i] = myLower[i] + myDx[i] * point[i];
		}
	}

	PDE::Solution::Solution(const PDE & pde, const LinearHyperMatrix & dim, double time) : myPde(pde), myVector(new HyperVector(dim)), myTime(time)
	{
	}

	PDE::Solution & PDE::Solution::operator+=(double val)
	{
		std::vector<size_t> point(myPde.myDimension, 0);

		do
		{
			(*myVector)(point) += val;
		}
		while (myVector->dim().next(point));

		return *this;
	}

	PDE::Solution & PDE::Solution::operator+=(const Payoff & val)
	{
		std::vector<size_t> point(myPde.myDimension, 0);
		std::vector<double> location(myPde.myDimension, 0);

		do
		{
			myPde.location(point, location);
			const double value = val(location);
			(*myVector)(point) += value;
		}
		while (myVector->dim().next(point));

		return *this;
	}

	PDE::Solution & PDE::Solution::solveTo(double t)
	{
		assert(t < myTime);

		const double step = myTime - t;

		MatrixPtr expAt = expViaGSL(step, myPde.myGenerator->mat());
		// MatrixPtr expAt = expViaTheta(step, myPde.myGenerator->mat(), 4, 0.5);
		// MatrixPtr expAt = expViaExplicit(step, myPde.myGenerator->mat(), 10);
		// MatrixPtr expAt = expViaEigenvalues(step, myPde.myGenerator->mat());

		VectorPtr result(gsl_vector_alloc(expAt->size1), VectorDeleter());

		gsl_blas_dgemv (CblasNoTrans, 1.0, expAt.get(), myVector->vect().get(), 0.0, result.get());

		// this is not a swap operation!
		(*myVector) = result;
		myTime = t;

		return *this;
	}
	
	PDE::Solution & PDE::Solution::solveTo2(double t)
	{
		assert(t < myTime);

		const double step = myTime - t;

		VectorPtr result = expViaTraditional(step, myPde.myGenerator->mat(), 10, myVector->vect());
		(*myVector) = result;
		myTime = t;

		return *this;
	}

	double PDE::Solution::time() const
	{
		return myTime;
	}

	const LinearHyperMatrix & PDE::Solution::dim() const
	{
		return myVector->dim();
	}

	double PDE::Solution::value(const std::vector<size_t> & point) const
	{
		return (*myVector)(point);
	}

	class CallPayoff : public Payoff
	{
	public:
		virtual double operator()(const std::vector<double> & ) const;
	};

	double CallPayoff::operator()(const std::vector<double> & x) const
	{
		return std::max(exp(x[0]) - 1.0, 0.0);
	}

	void pde_try()
	{
		const double sigma = 0.3;

		BrownianMotionWithDrift bmwd(-0.5 * sigma * sigma, sigma);

		const double time = 1.0;
		const double stddev = sigma * sqrt(time) * 3.0;

		std::vector<double> lower(1, -stddev);
		std::vector<double> upper(1, +stddev);
		std::vector<size_t> sizes(1, 201);

		PDE pde(bmwd, lower, upper, sizes);

		PDE::Solution s = pde.solve(time);
		s += CallPayoff();

		s.solveTo(0.0);

		const size_t size = s.dim().size(0);
		const size_t dim  = s.dim().dim();
		std::vector<size_t> point(dim, 0);
		std::vector<double> loc(dim);
		for (size_t i = 0; i < size; ++i)
		{
			point[0] = i;
			pde.location(point, loc);
			const double val = s.value(point);

			const double fwd = exp(loc[0]);

			const double bs = fwd * blackCallPrice(log(1.0) - loc[0], sigma * sqrt(time));

			printf("%g, %g, %g\n", fwd, val, bs);
		}
	}

}
