package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 *
 * Select a subset of a TimeSeries
 */
public class Select implements TimeSeries
{
    private TimeSeries m_Value;
    private int m_First;
    private int m_Last;
    
    /**
     * Select a subset of a TimeSeries
     *
     * @param value Original time series
     * @param first initial point (inclusive)
     * @param last final point (exclusive)
     */
    public Select(TimeSeries value, int first, int last)
    {
        m_Value = value;
        m_First = first;
        m_Last  = last;
    }

    public List<Date> dates(Memoizer<Schedule, Date> storageDates)
    {
        List<Date> valueDates = m_Value.dates(storageDates);
        
        // null is NOT valid
        
        List<Date> theDates = valueDates.subList(m_First, m_Last);
        return storageDates.put(this, theDates);
    }

    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        throw new RuntimeException("WTF");
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> values = storage.get(m_Value);
        storage.put(this, values.subList(m_First, m_Last));
    }

    public List<TimeSeries> children()
    {
        return Arrays.asList(m_Value);
    }


}
