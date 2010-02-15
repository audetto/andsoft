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

package asi.elves;

import asi.elves.script.*;
import java.util.*;
import org.quantlib.*;


/**
 *  Interpreter for ELVES
 */
public class ScriptEngine
{
    private TimeSeries m_root;
    private List<TimeSeries> m_allNodes;

    private List<java.util.Date> m_rootDates;
    private Memoizer<asi.elves.script.Schedule, java.util.Date> m_allSchedules = new Memoizer<asi.elves.script.Schedule, java.util.Date>();
    private List<java.util.Date> m_mergedDates;

    private int m_numberOfStocks;

    public ScriptEngine(TimeSeries root)
    {
        m_root = root;
        m_allNodes = getAllNodes();

        m_rootDates = root.checkAndStoreDates(m_allSchedules);

        if (m_rootDates == null)
            throw new RuntimeException("Missing dates on root node");

        m_mergedDates = mergeAllDates();
        m_numberOfStocks = checkNumberOfStocks();
    }

    private int checkNumberOfStocks()
    {
        int maxId = -1;
        for (TimeSeries child : m_allNodes)
        {
            if (child instanceof asi.elves.script.Stock)
            {
                asi.elves.script.Stock stock = (asi.elves.script.Stock)child;
                maxId = Math.max(maxId, stock.id());
            }
        }

        // return the NUMBER of stocks
        return maxId + 1;
    }

    private List<TimeSeries> getAllNodes()
    {
        List<TimeSeries> allNodes = new ArrayList<TimeSeries>();
        addChildren(allNodes, m_root);
        return allNodes;
    }

    private List<java.util.Date> mergeAllDates()
    {
        TreeSet<java.util.Date> dates = new TreeSet<java.util.Date>();
        for (TimeSeries node : m_allNodes)
        {
            List<java.util.Date> nodeDates = m_allSchedules.get(node);

            if (nodeDates == null)
                throw new RuntimeException("Missing dates on some node");

            dates.addAll(nodeDates);
        }

        List<java.util.Date> orderedDates = new ArrayList<java.util.Date>(dates);
        return orderedDates;
    }

    private void addChildren(List<TimeSeries> nodes, TimeSeries one)
    {
        if (nodes.contains(one))
            return;

        for (TimeSeries node : one.valueChildren())
        {
            addChildren(nodes, node);
        }

        nodes.add(one);
    }

    /**
     * Return all merged dates.
     *
     * @return dates required in the simulation
     */
    public List<java.util.Date> allDates()
    {
        return m_mergedDates;
    }

    /**
     * Print a list of all nodes and on which dates they provide a value
     */
    public void printNodeDates()
    {
        for (TimeSeries node : m_allNodes)
        {
            System.out.println(node.toString() + ": " + m_allSchedules.get(node).toString());
        }
    }

    private List<Double> valuePath(Path path)
    {
        /* There is actually no need to value only the unique nodes
         * in that order.
         * We could just value the root and whenever we need the value of a child,
         * 
         * {
         *     if (!memoizer.contains(node))
         *     {
         *         memoizer.put(node, node.values(path, memoizer));
         *     }
         *     List<Double> values = memoizer.get(node);
         * }
         * 
         * For unspecified reasons, we centralize everything here.
         * It would be nice to forbit a node from calling values() directly.
         */

        Memoizer<TimeSeries, Double> storage = new Memoizer<TimeSeries, Double>();
        for (TimeSeries node : m_allNodes)
        {
            node.values(path, storage, m_allSchedules);
        }

        return storage.get(m_root);
    }

    static org.quantlib.Date toQLDate(java.util.Date date)
    {
        int day = date.getDate();
        int month = date.getMonth() + 1;
        int year = date.getYear() + 1900;

        Month m = Month.swigToEnum(month);

        return new org.quantlib.Date(day, m, year);
    }

    class QLPayoff extends ExternalOption
    {
        @Override
        public DateVector fixingDates()
        {
            DateVector dv = new DateVector();

            for (java.util.Date date : m_mergedDates)
            {
                dv.add(toQLDate(date));
            }
            return dv;
        }

        @Override
        public long basisSystemDimension()
        {
            return 0;
        }

        @Override
        public void value(final Matrix path, Array payments, Array exercises, ArrayVector states)
        {
            Path p = new Path()
            {
                public double getValue(java.util.Date date, int id)
                {
                    int pos = m_mergedDates.indexOf(date);
                    return path.get(id, pos);
                }
            };

            List<Double> values = valuePath(p);

            int numberOfValues = values.size();

            for (int i = 0; i < numberOfValues; ++i)
            {
                java.util.Date date = m_mergedDates.get(i);
                int pos = m_mergedDates.indexOf(date);
                payments.set(pos, values.get(i));
            }
        }
    }

    public ExternalOption getQLOption()
    {
        return new QLPayoff();
    }

}
