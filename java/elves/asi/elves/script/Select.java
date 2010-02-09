package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 *
 * Select a subset of a TimeSeries
 */
public class Select extends AbstractTimeSeries
{
    private TimeSeries m_value;
    private int m_first;
    private int m_last;
    
    /**
     * Select a subset of a TimeSeries
     *
     * @param value Original time series
     * @param first initial point (inclusive)
     * @param last final point (exclusive)
     */
    public Select(TimeSeries value, int first, int last)
    {
        m_value = value;
        m_first = first;
        m_last  = last;
    }

    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        /* this is a leaf since we cannot inverse the transformation
         * dates[first, last)
         */
        List<Date> valueDates = m_value.checkAndStoreDates(storageDates);
        
        // null is NOT valid and a NPE will be thrown
        
        List<Date> theDates = valueDates.subList(m_first, m_last);
        return theDates;
    }

    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        // this is not supposed to be called
        // since datesImpl() never returns null
        throw new RuntimeException("WTF");
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> values = storage.get(m_value);
        storage.put(this, values.subList(m_first, m_last));
    }

    public List<TimeSeries> children()
    {
        return Arrays.asList(m_value);
    }


}
