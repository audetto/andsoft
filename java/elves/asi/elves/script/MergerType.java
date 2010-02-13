/*
 *   Copyright 2004-2010 AndSoft Inc.  All Rights Reserved.
 *
 *   This file is part of the AndSoft Library.
 *   http://www.odetti.it/andrea/index.htm
 *
 *   AndSoft is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   AndSoft is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with AndSoft.  If not, see <http://www.gnu.org/licenses/>.
 */

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
                return null;

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
