#ifndef ASI_BROWNIAN_MOTION_WITH_DRIFT_H
#define ASI_BROWNIAN_MOTION_WITH_DRIFT_H

#include <ql/stochasticprocess.hpp>

namespace ASI
{

	class BrownianMotionWithDrift : public QuantLib::StochasticProcess
	{
	public:
		BrownianMotionWithDrift(const double drift, const double volatility);
		
		virtual QuantLib::Size size() const;
		virtual QuantLib::Disposable<QuantLib::Array> initialValues() const;
		virtual QuantLib::Disposable<QuantLib::Array> drift(QuantLib::Time t, const QuantLib::Array& x) const;
		virtual QuantLib::Disposable<QuantLib::Matrix> diffusion(QuantLib::Time t, const QuantLib::Array& x) const;
		
	private:
		const double myDrift;
		const double myVolatility;
	};
	
}

#endif
