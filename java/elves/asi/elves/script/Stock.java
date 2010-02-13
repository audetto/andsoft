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
 * Block representing the path of a stock
 */
public class Stock extends Leaf
{
    private final int m_id;

    /**
     * Create a time series for a stock
     *
     * @param id Stock id
     * @param dates Observation dates
     */
    public Stock(int id, Schedule dates)
    {
        super(dates);
        m_id = id;
    }

    /**
     * Create a time series for a stock without a stock
     *
     * @param id Stock id
     */
    public Stock(Integer id)
    {
        this(id, null);
    }

    public int id()
    {
        return m_id;
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        List<Double> stock = new ArrayList<Double>();

        List<Date> dates = storageDates.get(this);

        for (Date date : dates)
        {
            stock.add(path.getValue(date, m_id));
        }

        storage.put(this, stock);
    }

}
