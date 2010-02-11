package asi.elves;

import java.util.*;

/**
 *
 * The simulated path
 */
public class Path
{
    private Map<Date, Map<Integer, Double> > m_data;

    public Path(Map<Date, Map<Integer, Double> > data)
    {
        m_data = data;
    }

    /**
     * Value of a stock on a given date.
     * <p>
     * This is the main access to the stochastic path.
     *
     */
    public double getValue(Date date, int id)
    {
        Map<Integer, Double> stocks = m_data.get(date);
        Double value = stocks.get(id);
        return value;
    }

}
