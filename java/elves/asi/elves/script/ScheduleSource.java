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
import asi.elves.*;

/**
 * Helper function for a node that does not propagate the schedule
 * from the children upward
 */
public abstract class ScheduleSource extends Function
{
    public ScheduleSource(List<TimeSeries> children, MergerType merger)
    {
        super(children, merger);
    }

    /**
     * This function throws always.
     * It is not supposed to be called.
     * <p>
     * The children of this node must provide a schedule. It cannot come from above
     *
     */
    @Override
    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        // this is not supposed to be called
        // since datesImpl() never returns null
        throw new RuntimeException("WTF");
    }

    @Override
    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        List<Date> valueDates = super.datesImpl(storageDates);

        if (valueDates == null)
            throw new RuntimeException("Missing dates for " + this);

        return valueDates;
    }
}
