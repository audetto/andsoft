package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 * Block representing the path of a stock
 */
public class Stock extends Leaf
{
    private final int m_id;

    /**
     * Create a time series for a stock
     *
     * @param id Stock id
     * @param dates Observation dates
     */
    public Stock(int id, Schedule dates)
    {
        super(dates);
        m_id = id;
    }

    /**
     * Create a time series for a stock without a stock
     *
     * @param id Stock id
     */
    public Stock(Integer id)
    {
        this(id, null);
    }

    public int id()
    {
        return m_id;
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> stock = new ArrayList<Double>();

        List<Date> dates = storageDates.get(this);

        for (Date date : dates)
        {
            stock.add(path.getValue(date, m_id));
        }

        storage.put(this, stock);
    }

}
