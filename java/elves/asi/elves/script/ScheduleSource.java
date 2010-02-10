package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 * Helper function for a node that does not propagate the schedule
 * from the children upward
 */
public abstract class ScheduleSource extends Function
{
    public ScheduleSource(List<TimeSeries> children, MergerType merger)
    {
        super(children, merger);
    }

    /**
     * This function throws always.
     * It is not supposed to be called.
     * <p>
     * The children of this node must provide a schedule. It cannot come from above
     *
     */
    @Override
    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        // this is not supposed to be called
        // since datesImpl() never returns null
        throw new RuntimeException("WTF");
    }

    @Override
    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        List<Date> valueDates = super.datesImpl(storageDates);

        if (valueDates == null)
            throw new RuntimeException("Missing dates for " + this);

        return valueDates;
    }
}
