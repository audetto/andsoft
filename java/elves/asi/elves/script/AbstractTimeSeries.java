package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 * Helper class to save the result of datesImpl() to the memoizer.
 *
 * @author andrea
 */
public abstract class AbstractTimeSeries implements TimeSeries
{
    /**
     * Raw unprotected implemetation of checkAndStoreDates().
     * <p>
     * Do not use the memoizer here, just call checkAndStoreDates() on the children.
     * <p>
     * NEVER call datesImpl(), ALWAYS call checkAndStoreDates()!
     *
     */
    protected abstract List<Date> datesImpl(Memoizer<Schedule, Date> storageDates);

    /**
     * Full implementation of dates().
     * <p>
     * It will store the dates to the memoizer (which will throw if other different dates
     * are already present).
     */
    final public List<Date> checkAndStoreDates(Memoizer<Schedule, Date> storageDates)
    {
        /* we must not stop if the node has already a set of dates (they could have been forced).
         * we still have to check the different dates will not be force elsewhere */

        List<Date> theseDates = datesImpl(storageDates);
        return storageDates.put(this, theseDates);
    }

    /**
     * Force the dates on this object and all its children.
     */
    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        storageDates.put(this, theDates);
        for (Schedule child : dateChildren())
        {
            if (child != null)
                child.forceDates(theDates, storageDates);
        }
    }

    /**
     * Return the nodes used to compute this node's dates
     *
     * @return the valueChildren()
     */
    public List<? extends Schedule> dateChildren()
    {
        return valueChildren();
    }

}
