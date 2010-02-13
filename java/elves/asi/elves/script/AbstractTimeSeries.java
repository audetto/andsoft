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
 * Helper class to save the result of datesImpl() to the memoizer.
 *
 * @author andrea
 */
public abstract class AbstractTimeSeries implements TimeSeries
{
    /**
     * Raw unprotected implemetation of checkAndStoreDates().
     * <p>
     * Do not use the memoizer here, just call checkAndStoreDates() on the children.
     * <p>
     * NEVER call datesImpl(), ALWAYS call checkAndStoreDates()!
     *
     */
    protected abstract List<Date> datesImpl(Memoizer<Schedule, Date> storageDates);

    /**
     * Full implementation of dates().
     * <p>
     * It will store the dates to the memoizer (which will throw if other different dates
     * are already present).
     */
    final public List<Date> checkAndStoreDates(Memoizer<Schedule, Date> storageDates)
    {
        /* we must not stop if the node has already a set of dates (they could have been forced).
         * we still have to check the different dates will not be force elsewhere */

        List<Date> theseDates = datesImpl(storageDates);
        return storageDates.put(this, theseDates);
    }

    /**
     * Force the dates on this object and all its children.
     */
    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates)
    {
        storageDates.put(this, theDates);
        for (Schedule child : dateChildren())
        {
            if (child != null)
                child.forceDates(theDates, storageDates);
        }
    }

    /**
     * Return the nodes used to compute this node's dates
     *
     * @return the valueChildren()
     */
    public List<? extends Schedule> dateChildren()
    {
        return valueChildren();
    }

}
