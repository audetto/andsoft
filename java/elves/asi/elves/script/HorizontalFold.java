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
 * value = f(f(f(f(f(initialValue, x1), x2), x3), x4), x5)
 *
 * @author Andrea Odetti <mariofutire@gmail.com>
 */
public class HorizontalFold extends TimewiseFunction
{
    private BinaryFunction m_func;
    private double m_initialValue;

    /**
     * FoldLeft func timewise
     *
     * @param func binary function
     * @param initialValue start value of the fold
     */
    public HorizontalFold(BinaryFunction func, double initialValue, List<TimeSeries> values)
    {
        super(values, MergerType.EXACT);
        m_func = func;
        m_initialValue = initialValue;
    }

    public double computeSingleValue(int datePos, Date date, List<Double> values)
    {
        double value = m_initialValue;
        for (Double v : values)
        {
            value = m_func.value(value, v);
        }

        return value;
    }
}
