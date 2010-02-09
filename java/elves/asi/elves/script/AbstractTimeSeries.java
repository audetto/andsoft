package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 * Helper class to protect dates(), by first checking the memoizer,
 * than storing the results.
 *
 * @author andrea
 */
public abstract class AbstractTimeSeries implements TimeSeries
{
    /**
     * Raw unprotected implemetation of dates().
     * <p>
     * Do not use the memoizer here,
     * just call dates() on the children.
     * <p>
     * NEVER call datesImpl(), ALWAYS call dates()!
     *
     */
    protected abstract List<Date> datesImpl(Memoizer<Schedule, Date> storageDates);

    /**
     * Safe implementation of date().
     */
    final public List<Date> dates(Memoizer<Schedule, Date> storageDates)
    {
        /* we must not stop to check if the node has already a set of dates
         * they could have been forced.
         * we still have to check different dates will not be force elsewhere */

        List<Date> theseDates = datesImpl(storageDates);
        return storageDates.put(this, theseDates);
    }

}
