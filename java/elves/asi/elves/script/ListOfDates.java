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

    public List<Date> dates(Memoizer<Schedule, Date> storageDates)
    {
        return storageDates.put(this, m_dates);
    }

}
