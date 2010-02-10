package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * This class represents a List of Dates
 */
public interface Schedule
{
    public List<Date> checkAndStoreDates(Memoizer<Schedule, Date> storageDates);

    public List<? extends Schedule> dateChildren();

    /**
     * Store the given date for this node and all its children
     * <p>
     * Unless this node is a leaf wrt the schedule
     */

    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates);
}
