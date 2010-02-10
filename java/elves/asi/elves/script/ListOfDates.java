package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * Helper to create a schedule from a list of dates
 */
public class ListOfDates implements Schedule
{
    private List<Date> m_dates;
    public ListOfDates(List<Date> dates)
    {
        m_dates = dates;
    }

    public List<Date> checkAndStoreDates(Memoizer<Schedule, Date> storageDates)
    {
        return storageDates.put(this, m_dates);
    }

    public List<? extends Schedule> dateChildren()
    {
        return Collections.emptyList();
    }

    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        throw new RuntimeException("WTF");
    }
}
