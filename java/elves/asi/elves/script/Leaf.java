package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 * Helper implementation for an object without children
 */
public abstract class Leaf extends AbstractTimeSeries
{
    private Schedule m_schedule;

    public Leaf(Schedule schedule)
    {
        m_schedule = schedule;
    }

    /**
     * This node has no value children
     *
     * @return An empty list
     */
    public List<TimeSeries> valueChildren()
    {
        return Collections.emptyList();
    }

    /**
     * This node gets the dates from the schedule passed in the constructor
     *
     */
    @Override
    public List<? extends Schedule> dateChildren()
    {
        return Arrays.asList(m_schedule);
    }

    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        if (m_schedule == null)
            return null;

        List<Date> theDates = m_schedule.checkAndStoreDates(storageDates);
        return theDates;
    }

}
