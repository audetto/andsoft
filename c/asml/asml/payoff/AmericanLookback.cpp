/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <asml/payoff/AmericanLookback.h>

using namespace QuantLib;

namespace ASI
{
    
    AmericanLookback::AmericanLookback() : m_euribor3m(new Euribor3M(m_termStructure))
    {
        
    }
    
    std::string AmericanLookback::name() const
    {
        return "AmericanLookback";
    }
    
    std::string AmericanLookback::description() const
    {
        return name();
    }
    
    Size AmericanLookback::basisSystemDimension() const
    {
        return 2;
    }
    
    void AmericanLookback::operator()(ValuationData & data) const
    {
        const Size numberOfTimes  = data.numberOfTimes();
        const Size numberOfAssets = data.numberOfAssets();
        
        double runningMaximum = 1.0;
        for (Size i = 0; i < numberOfTimes; ++i)
        {
            
            const Handle<YieldTermStructure> & yieldTermStructure = data.getYieldTermStructure(i);
            const Date & referenceDate = yieldTermStructure->referenceDate();
            
            // relink term structure to actual value
            m_termStructure.linkTo(yieldTermStructure.currentLink());
            
            const Date spotDate = m_euribor3m->fixingCalendar().adjust(referenceDate);
            const Rate spotEuribor = m_euribor3m->fixing(spotDate);
            
            double thisSum = 0.0;
            for (Size j = 0; j < numberOfAssets; ++j)
                thisSum += data.getAssetValue(i, j);
            thisSum /= numberOfAssets;
            
            //            if (data.getAssetValue(i, 0) < 1)
            //                break;
            
            runningMaximum = std::max(runningMaximum, thisSum);
            
            Array financialCoordinates(basisSystemDimension());
            
            const double payoff = runningMaximum - thisSum;
            financialCoordinates[0]   = runningMaximum / thisSum;
            financialCoordinates[1]   = thisSum;
            
            double exercise = payoff;
            double payment = 0.0;
            
            // this is only to be able to price the european version.
            if (i == numberOfTimes - 1)
                std::swap(exercise, payment);
            
            data.setPayoffValue(i, payment);
            data.setExerciseData(i, exercise, financialCoordinates);
        }
    }
    
}
