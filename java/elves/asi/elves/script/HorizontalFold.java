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

/**
 * Value in each time is a function of the children's values.
 * <p>
 * There must be at least a child
 * value = f(f(f(f(x1, x2), x3), x4), x5)
 *
 * @author Andrea Odetti <mariofutire@gmail.com>
 */
public class HorizontalFold extends TimewiseFunction
{
    private BinaryFunction m_func;

    /**
     * FoldLeft func timewise
     *
     * @param func binary function
     */
    public HorizontalFold(BinaryFunction func, List<TimeSeries> values)
    {
        super(values, MergerType.EXACT);
        m_func = func;
    }

    public double computeSingleValue(int datePos, Date date, List<Double> values)
    {
        if (values.isEmpty())
            throw new RuntimeException("Cannot fold empty list " + this);

        double value = values.get(0);
        for (int i = 1; i < values.size(); ++i)
        {
            value = m_func.value(value, values.get(i));
        }

        return value;
    }
}
