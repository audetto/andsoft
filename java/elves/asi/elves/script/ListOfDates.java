package asi.elves.script;

import java.util.*;

/**
 *
 * @author andrea
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
