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
 *
 * Select a subset of a TimeSeries
 */
public class Select extends ScheduleSource
{
    private TimeSeries m_value;
    private int m_first;
    private int m_last;
    
    /**
     * Select a subset of a TimeSeries
     *
     * @param value Original time series
     * @param first initial point (inclusive)
     * @param last final point (exclusive)
     */
    public Select(TimeSeries value, int first, int last)
    {
        super(Arrays.asList(value), MergerType.EXACT);
        m_value = value;
        m_first = first;
        m_last  = last;
    }

    @Override
    protected List<Date> datesImpl(Memoizer<Schedule, Date> storageDates)
    {
        /* this is a leaf since we cannot inverse the transformation
         * dates[first, last)
         */
        List<Date> valueDates = super.datesImpl(storageDates);

        // this is guaranteed to be non null

        List<Date> theDates = valueDates.subList(m_first, m_last);
        return theDates;
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> values = storage.get(m_value);
        storage.put(this, values.subList(m_first, m_last));
    }

}
