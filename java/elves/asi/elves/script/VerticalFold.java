/*
 * Copyright 2010 AndSoft Inc. All Rights Reserved.
 * 
 * This file is part of The AndSoft Library.
 * http://www.odetti.it/andrea/index.htm
 * 
 * The AndSoft Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * The AndSoft Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with The AndSoft Library.  If not, see <http://www.gnu.org/licenses/>.
 */

package asi.elves.script;

import java.util.*;
import asi.elves.*;

/**
 *
 * @author Andrea Odetti <mariofutire@gmail.com>
 */
public class VerticalFold extends Function
{
    private TimeSeries m_child;
    private int m_maximumSize;
    private ArrayFunction m_func;

    /**
     * Function to compute the vertical "fold" of the last maximumSize values.
     * <p>
     * In the first few dates, the actual number of values passed to m_func
     * will be less than the maximum.
     *
     * @param value single child of this class
     * @param func
     * @param maximumSize how many values to pass (i.e. last 10)
     */

    public VerticalFold(TimeSeries value, ArrayFunction func, int maximumSize)
    {
        super(Arrays.asList(value), MergerType.EXACT);
        m_child = value;
        m_maximumSize = maximumSize;
        m_func = func;
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storageValues, Memoizer<Schedule, Date> storageDates)
    {
        List<Date> theDates = storageDates.get(this);
        int numberOfDates = theDates.size();

        List<Double> values = storageValues.get(m_child);

        List<Double> theValues = new ArrayList<Double>(numberOfDates);

        Queue<Double> fifo = new ArrayDeque<Double>();

        for (int i = 0; i < numberOfDates; ++i)
        {
            fifo.add(values.get(i));
            if (fifo.size() > m_maximumSize)
            {
                // we only keep up to maximumSize elements
                fifo.remove();  // removes the first
            }

            double value = m_func.value(fifo);

            theValues.set(i, value);
        }

        storageValues.put(this, theValues);
    }

}
