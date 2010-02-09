package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 * Node for a constant number
 */
public class Constant extends Leaf
{
    private final double m_value;

    /**
     * Create a constant time series
     *
     */
    public Constant(double value, Schedule dates)
    {
        super(dates);
        m_value = value;
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> constant = new ArrayList<Double>();

        List<Date> dates = storageDates.get(this);

        for (Date date : dates)
        {
            constant.add(m_value);
        }

        storage.put(this, constant);
    }

}
