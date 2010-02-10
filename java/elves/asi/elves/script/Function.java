package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 * Helper class to handle most common cases.
 */
public abstract class Function extends AbstractTimeSeries
{
    private List<TimeSeries> m_children;
    private MergerType m_merger;

    /**
     *
     * @param valueChildren
     * @param merger Rule used to merge the children's dates.
     */
    public Function(List<TimeSeries> children, MergerType merger)
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

        List<Schedule> nullChildren = new ArrayList<Schedule>();
        List<List<Date> > stdChildren = new ArrayList<List<Date> >();
        
        for (Schedule child : dateChildren())
        {
            if (child != null)
            {
                List<Date> theseDates = child.checkAndStoreDates(storageDates);
                if (theseDates == null)
                    nullChildren.add(child);
                else
                    stdChildren.add(theseDates);
            }
        }
        
        List<Date> mergedDates = m_merger.mergeDates(stdChildren);

        if (mergedDates != null)
        {
            for (Schedule child : nullChildren)
            {
                child.forceDates(mergedDates, storageDates);
            }
        }
        
        return mergedDates;
    }

    public List<TimeSeries> valueChildren()
    {
        return m_children;
    }
}
