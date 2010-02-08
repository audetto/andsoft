package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * Block used by a parent block to return many results
 */
public class InnerBlock implements TimeSeries
{
    private TimeSeries m_parent;
    private Schedule m_schedule;

    public InnerBlock(TimeSeries parent, Schedule schedule)
    {
        m_parent = parent;
        m_schedule = schedule;
    }

    public List<Date> dates()
    {
        return m_schedule.dates();
    }

    public void values(Path path, Memoizer<Double> storage)
    {
        // the parent must have added it already.
        assert storage.get(this) != null : this;
    }

    public List<TimeSeries> children()
    {
        return Arrays.asList(m_parent);
    }


}
