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
     * It is not required to use the memoizer directly.
     * NEVER call datesImple(), ALWAYS call dates()!
     *
     */
    protected abstract List<Date> datesImpl(Memoizer<Schedule, Date> storageDates);

    /**
     * Safe implementation of date().
     * <p>
     * It first checks to see if this object has already calculated it,
     * then caches the result before returning.
     */
    final public List<Date> dates(Memoizer<Schedule, Date> storageDates)
    {
        List<Date> theseDates = storageDates.get(this);
        if (theseDates != null)
            return theseDates;

        theseDates = datesImpl(storageDates);
        return storageDates.put(this, theseDates);
    }

}
