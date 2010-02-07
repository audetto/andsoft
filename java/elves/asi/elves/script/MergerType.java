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

        public List<Date> mergeDates(List<? extends Schedule> schedules)
        {
            if (schedules.isEmpty())
                return new ArrayList<Date>();

            List<Date> dates = schedules.get(0).dates();
            for (int i = 1; i < schedules.size(); ++i)
            {
                if (!dates.equals(schedules.get(i).dates()))
                    throw new RuntimeException("Dates do not allow for EXACT merge.");
            }

            return dates;
        }

    };

    /**
     * Merges many schedules into one
     *
     */
    public abstract List<Date> mergeDates(List<? extends Schedule> schedules);
}
