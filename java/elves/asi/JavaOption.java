package asi;

import asi.elves.*;
import asi.elves.script.*;
import java.util.*;
import org.quantlib.*;

class AsianBasket extends ExternalOption
{

    @Override
    public DateVector fixingDates()
    {
        DateVector dv = new DateVector();
        dv.add(new org.quantlib.Date(5, Month.May, 2010));
        dv.add(new org.quantlib.Date(5, Month.May, 2011));
        return dv;
    }

    @Override
    public long basisSystemDimension()
    {
        return 0;
    }

    /**
     * Pays the average of all underlying (so far) at every fixing date.
     */
    @Override
    public void value(Matrix path, TermStructureVector yieldCurves, Array payments, Array exercises, ArrayVector states)
    {
        long numberOfAssets = path.rows();
        long numberOfTimes = path.columns();
        double sum = 0.0;

        for (long i = 0; i < numberOfTimes; ++i)
        {
            for (long j = 0; j < numberOfAssets; ++j)
            {
                sum += path.get(j, i);
            }
            double payoff = sum / (numberOfAssets * (i + 1));
            payments.set(i, payoff);
        }
    }
}

public class JavaOption
{

    static
    {
        try
        {
            System.loadLibrary("QuantLibJNI");
        }
        catch (RuntimeException e)
        {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws Exception
    {

        QuantLib.enableTracing();

        long beginTime = System.currentTimeMillis();

        // our option
        double underlying = 100.0;
        double riskFreeRate = 0.06;
        double dividendYield = 0.00;
        double volatility = 0.2;

        org.quantlib.Date todaysDate = new org.quantlib.Date(15, Month.May, 1998);
        org.quantlib.Date settlementDate = new org.quantlib.Date(17, Month.May, 1998);
        Settings.instance().setEvaluationDate(todaysDate);

        DayCounter dayCounter = new Actual365Fixed();
        org.quantlib.Calendar calendar = new TARGET();

        // write column headings
        String fmt = "\n%-35s %-14s %-14s %-14s\n";
        System.out.printf(fmt, "Method", "European", "Bermudan", "American");
        System.out.println("============================================================================");

        // define the underlying asset and the yield/dividend/volatility curves
        QuoteHandle underlyingH = new QuoteHandle(new SimpleQuote(underlying));
        YieldTermStructureHandle flatTermStructure =
                new YieldTermStructureHandle(new FlatForward(
                settlementDate, riskFreeRate, dayCounter));
        YieldTermStructureHandle flatDividendYield =
                new YieldTermStructureHandle(new FlatForward(
                settlementDate, dividendYield, dayCounter));
        BlackVolTermStructureHandle flatVolatility =
                new BlackVolTermStructureHandle(new BlackConstantVol(
                settlementDate, calendar, volatility, dayCounter));

        BlackScholesMertonProcess stochasticProcess =
                new BlackScholesMertonProcess(underlyingH,
                flatDividendYield,
                flatTermStructure,
                flatVolatility);

        StochasticProcessVector vsp = new StochasticProcessVector();
        vsp.add(stochasticProcess);
        Matrix corr = new Matrix(1, 1);
        corr.set(0, 0, 1.0);

        StochasticProcessArray asp = new StochasticProcessArray(vsp, corr);

        asi.elves.script.Schedule dates = new ListOfDates(Arrays.asList(
                new java.util.Date(101, 6, 1),
                new java.util.Date(102, 6, 1)));

        TimeSeries number1 = new Constant(100, dates);
        TimeSeries number2 = new asi.elves.script.Stock(0);

        Sort maximum = new Sort(Arrays.asList(number1, number2));

        ScriptEngine engine = new ScriptEngine(maximum.sortedValues().get(1));

        ExternalOption payoff = engine.getQLOption();

//        AsianBasket asianBasket = new AsianBasket();
        Instrument option = payoff.instrument();

        fmt = "%34s %13.9f %13.9f %13.9f\n";

        // Monte Carlo Method
        int timeSteps = 1;
        int mcSeed = 43;
        int nSamples = 10000;
        int maxSamples = 10000; // 2^20

        String method = "MC (crude)";
        option.setPricingEngine(
                new MCPathBasketEngine(asp,
                "PseudoRandom", timeSteps, QuantLib.nullInt(),
                false, false,
                nSamples, QuantLib.nullDouble(), maxSamples, mcSeed));
        System.out.printf(fmt, new Object[]
                {
                    method,
                    option.NPV(),
                    Double.NaN,
                    Double.NaN
                });

        method = "MC (Sobol)";
        option.setPricingEngine(
                new MCPathBasketEngine(asp,
                "LowDiscrepancy", timeSteps, QuantLib.nullInt(),
                false, false,
                nSamples, QuantLib.nullDouble(), maxSamples, mcSeed));
        System.out.printf(fmt, new Object[]
                {
                    method,
                    option.NPV(),
                    Double.NaN,
                    Double.NaN
                });

        long msecs = (System.currentTimeMillis() - beginTime);
        System.out.println("Run completed in " + msecs + " ms.");

    }
}

