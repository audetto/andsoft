package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 * Helper implementation for an object without children
 */
public abstract class Leaf implements TimeSeries
{
    private Schedule m_schedule;

    public Leaf(Schedule schedule)
    {
        m_schedule = schedule;
    }

    public List<Date> dates(Memoizer<Schedule, Date> storageDates)
    {
        return storageDates.put(this, m_schedule.dates(storageDates));
    }

    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        storageDates.put(this, theDates);
        // this TimeSeries does not have children
    }

    public List<TimeSeries> children()
    {
        return Collections.emptyList();
    }

}
