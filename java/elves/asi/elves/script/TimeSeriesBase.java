package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 * Helper class to handle most common cases.
 */
public abstract class TimeSeriesBase extends AbstractTimeSeries
{
    private List<TimeSeries> m_children;
    private MergerType m_merger;

    /**
     *
     * @param children
     * @param merger Ruled used to merge the children's dates.
     */
    public TimeSeriesBase(List<TimeSeries> children, MergerType merger)
    {
        m_children = children;
        m_merger = merger;
    }

    /**
     * This is the main implementation of the date propagation.
     * <p>
     * If the children with non null dates agree on a common schedule,
     * this schedule is forced on all null children.
     */
    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        /*
         * Key algorithm for date propagation.
         * If a parent (i.e. this) has some children without dates,
         * it will force on them the dates that would come
         * from merging all dates of the other children.
         * If no child has a date, then it will return null,
         * and wait for its parent to call forceDates() on this.
         */

        List<TimeSeries> nullChildren = new ArrayList<TimeSeries>();
        List<List<Date> > stdChildren = new ArrayList<List<Date> >();
        
        for (TimeSeries child : children())
        {
            List<Date> theseDates = child.dates(storageDates);
            if (theseDates == null)
                nullChildren.add(child);
            else
                stdChildren.add(theseDates);
        }
        
        List<Date> mergedDates = m_merger.mergeDates(stdChildren);

        if (mergedDates != null)
        {
            for (TimeSeries child : children())
            {
                child.forceDates(mergedDates, storageDates);
            }
        }
        
        return mergedDates;
    }

    /**
     * Force the dates on this object and all its children.
     */
    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        storageDates.put(this, theDates);
        for (TimeSeries child : children())
        {
            child.forceDates(theDates, storageDates);
        }
    }

    public List<TimeSeries> children()
    {
        return m_children;
    }
}
