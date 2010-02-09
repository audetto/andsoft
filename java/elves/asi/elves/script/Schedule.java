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
}
