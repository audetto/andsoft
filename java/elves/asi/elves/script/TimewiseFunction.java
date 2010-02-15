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
 * Helper class to implement timewise functions,
 * where the result is a function of the values at a time
 *
 * @author Andrea Odetti <mariofutire@gmail.com>
 */
public abstract class TimewiseFunction extends Function
{
    public TimewiseFunction(List<TimeSeries> values, MergerType merger)
    {
        super(values, merger);
    }

    /**
     * Compute the value for this date as a function of the children's values
     *
     * @param datePos index of this date
     * @param date date
     * @param values value of all children
     * @return the vlaue for this date
     */
    public abstract double computeSingleValue(int datePos, Date date, List<Double> values);

    /**
     * Calls computeSingleValue for each date, passing a list of the values of all children
     *
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
            allValues.add(values);
        }

        List<Double> theValues = new ArrayList<Double>(numberOfDates);

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
            };

            double thisValue = computeSingleValue(i, theDates.get(i), valuesForDate);
            theValues.set(i, thisValue);
        }

        storageValues.put(this, theValues);
    }

}
