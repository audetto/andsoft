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
 *
 * Sort (date by date) a vector of time series in ascending order
 */
public class Sort extends Function
{
    private List<TimeSeries> m_inners = new ArrayList<TimeSeries>();

    public Sort(List<TimeSeries> values)
    {
        super(values, MergerType.EXACT);
        for (TimeSeries child : valueChildren())
        {
            // all inner blocks are defined on the same dates as this
            m_inners.add(new InnerBlock(this));
        }
    }

    /**
     * The blocks used to retrieve the return values of this object from the storage.
     *
     * @return The sorted values in ascending order.
     */
    public List<TimeSeries> sortedValues()
    {
        return m_inners;
    }

    /**
     * This block does not provide values.
     * <p>
     * Please access results via .sortedValues().
     */
    public void values(Path path, Memoizer<TimeSeries, Double> storageValues, Memoizer<Schedule, Date> storageDates)
    {
        List<Date> theDates = storageDates.get(this);
        int numberOfDates = theDates.size();

        List<TimeSeries> theChildren = valueChildren();
        final int numberOfValues = theChildren.size();

        final List<List<Double> > allValues = new ArrayList<List<Double> >();

        for (TimeSeries child : theChildren)
        {
            List<Double> values = storageValues.get(child);

            // shallow copy, since the storage returns const lists.
            allValues.add(new ArrayList<Double>(values));
        }

        for (int i = 0; i < numberOfDates; ++i)
        {
            final int thisDate = i;

            /* Anonymous class to represent a time slice (thisDate)
             * of the matrix allValues
             * Like a transpose
             */
            List<Double> valuesForDate = new AbstractList<Double>()
            {

                public Double get(int index)
                {
                    return allValues.get(index).get(thisDate);
                }

                public int size()
                {
                    return numberOfValues;
                }

                @Override
                public Double set(int index, Double element)
                {
                    return allValues.get(index).set(thisDate, element);
                }

            };

            Collections.sort(valuesForDate);
        }

        for (int j = 0; j < numberOfValues; ++j)
        {
            storageValues.put(m_inners.get(j), allValues.get(j));
        }

        List<Double> theValues = Collections.emptyList();
        storageValues.put(this, theValues);

    }

}
