#include <asml/process/brownianmotionwithdrift.h>

namespace ASI
{

	BrownianMotionWithDrift::BrownianMotionWithDrift(const double drift, const double volatility) : myDrift(drift), myVolatility(volatility)
	{
	}
	
	QuantLib::Size BrownianMotionWithDrift::size() const
	{
		return 1;
	}
	
	QuantLib::Disposable<QuantLib::Array> BrownianMotionWithDrift::initialValues() const
	{
		QuantLib::Array res(1, 0.0);
		return res;
	}
	
	QuantLib::Disposable<QuantLib::Array> BrownianMotionWithDrift::drift(QuantLib::Time t, const QuantLib::Array& x) const
	{
		QuantLib::Array res(1, myDrift);
		return res;
	}
	
	QuantLib::Disposable<QuantLib::Matrix> BrownianMotionWithDrift::diffusion(QuantLib::Time t, const QuantLib::Array& x) const
	{
		QuantLib::Matrix res(1, 1, myVolatility * myVolatility);
		return res;
	}
	
}
