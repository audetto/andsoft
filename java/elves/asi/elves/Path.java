package asi.elves;

import java.util.*;

/**
 *
 * The simulated path
 */
public class Path
{
    private Map<Date, Map<String, Double> > m_data;

    public Path(Map<Date, Map<String, Double> > data)
    {
        m_data = data;
    }

    /**
     * Value of a stock on a given date.
     * <p>
     * This is the main access to the stochastic path.
     *
     */
    public double getValue(Date date, String name)
    {
        Map<String, Double> stocks = m_data.get(date);
        Double value = stocks.get(name);
        return value;
    }

}
