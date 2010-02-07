package asi.elves;

import java.util.*;

/**
 *
 * @author andrea
 */
public class Path
{
    private Map<Date, Map<String, Double> > m_data;

    public Path(Map<Date, Map<String, Double> > data)
    {
        m_data = data;
    }

    public double getValue(Date date, String name)
    {
        Map<String, Double> stocks = m_data.get(date);
        Double value = stocks.get(name);
        return value;
    }

}
