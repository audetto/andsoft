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
 * Node for a constant number
 */
public class Constant extends Leaf
{
    private final double m_value;

    /**
     * Create a constant time series
     *
     */
    public Constant(double value, Schedule dates)
    {
        super(dates);
        m_value = value;
    }

    /**
     * Create a constant time series without schedule
     */
    public Constant(double value)
    {
        this(value, null);
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> constant = new ArrayList<Double>();

        List<Date> dates = storageDates.get(this);

        for (Date date : dates)
        {
            constant.add(m_value);
        }

        storage.put(this, constant);
    }

}
