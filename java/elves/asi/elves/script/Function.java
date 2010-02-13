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

import asi.elves.*;
import java.util.*;

/**
 * Helper class to handle most common cases.
 */
public abstract class Function extends AbstractTimeSeries
{
    private List<TimeSeries> m_children;
    private MergerType m_merger;

    /**
     *
     * @param valueChildren
     * @param merger Rule used to merge the children's dates.
     */
    public Function(List<TimeSeries> children, MergerType merger)
    {
        m_children = children;
        m_merger = merger;
    }

    /**
     * This is the main implementation of the date propagation.
     * <p>
     * If the children with non null dates agree on a common schedule,
     * this schedule is forced on all null children.
     */
    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        /*
         * Key algorithm for date propagation.
         * If a parent (i.e. this) has some children without dates,
         * it will force on them the dates that would come
         * from merging all dates of the other children.
         * If no child has a date, then it will return null,
         * and wait for its parent to call forceDates() on this.
         */

        List<Schedule> nullChildren = new ArrayList<Schedule>();
        List<List<Date> > stdChildren = new ArrayList<List<Date> >();
        
        for (Schedule child : dateChildren())
        {
            if (child != null)
            {
                List<Date> theseDates = child.checkAndStoreDates(storageDates);
                if (theseDates == null)
                    nullChildren.add(child);
                else
                    stdChildren.add(theseDates);
            }
        }
        
        List<Date> mergedDates = m_merger.mergeDates(stdChildren);

        if (mergedDates != null)
        {
            for (Schedule child : nullChildren)
            {
                child.forceDates(mergedDates, storageDates);
            }
        }
        
        return mergedDates;
    }

    public List<TimeSeries> valueChildren()
    {
        return m_children;
    }
}
