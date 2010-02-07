package asi.elves.script;

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

    public List<Date> dates()
    {
        return m_dates;
    }

}
