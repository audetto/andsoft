package asi.elves.script;

import java.util.*;

/**
 * How to handle merge many schedules
 */
public enum MergerType
{
    /**
     * Check that all schedules are EQUAL
     */
    EXACT
    {
        // check all schedules are equal and return the first.
        // nulls are ignored

        public List<Date> mergeDates(List<? extends List<Date> > schedules)
        {
            if (schedules.isEmpty())
                return Collections.emptyList();

            List<Date> dates = null;
            for (List<Date> schedule : schedules)
            {
                if (schedule != null)
                {
                    if (dates == null)
                    {
                        dates = schedule;
                    }
                    else
                    {
                        if (!dates.equals(schedule))
                            throw new RuntimeException("Dates do not allow for EXACT merge.");
                    }
                }
            }

            return dates;
        }

    };

    /**
     * Merges many schedules into one
     *
     */
    public abstract List<Date> mergeDates(List<? extends List<Date>> schedules);
}
