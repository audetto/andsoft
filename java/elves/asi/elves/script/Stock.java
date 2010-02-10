package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 * Block representing the path of a stock
 */
public class Stock extends Leaf
{
    private final String m_name;

    /**
     * Create a time series for a stock
     *
     * @param name Stock name
     * @param dates Observation dates
     */
    public Stock(String name, Schedule dates)
    {
        super(dates);
        m_name = name;
    }

    /**
     * Create a time series for a stock without a stock
     *
     * @param name Stock name
     */
    public Stock(String name)
    {
        this(name, null);
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> stock = new ArrayList<Double>();

        List<Date> dates = storageDates.get(this);

        for (Date date : dates)
        {
            stock.add(path.getValue(date, m_name));
        }

        storage.put(this, stock);
    }

}
