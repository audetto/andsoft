package asi.elves.script;

import java.util.*;

/**
 *
 * @author andrea
 */
public abstract class TimeSeriesBase implements TimeSeries
{
    private List<Date> m_schedule;
    private List<TimeSeries> m_children;

    public TimeSeriesBase(Schedule schedule)
    {
        m_schedule = schedule.dates();
        m_children = null;
    }
            
    public TimeSeriesBase(List<TimeSeries> children, MergerType merger)
    {
        m_children = children;
        m_schedule = merger.mergeDates(children);
    }

    public List<Date> dates()
    {
        return m_schedule;
    }

    public List<TimeSeries> children()
    {
        return m_children;
    }
}
