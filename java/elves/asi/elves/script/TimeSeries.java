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
 * Time series on a list of dates.
 * <p>
 * Elementary node in the AST. A list of (Date, double).
 */
public interface TimeSeries extends Schedule
{

    /**
     * Main function to calculate the values of this block.
     * <p>
     * This function must NOT call .values() on its arguments
     * but retrieve is from the Memoizer.
     *
     * The Memoizer will store a List(Double) corresponding to the values
     * returned by values().
     *
     * @param path The simulated path
     * @param storageValues Where to store results (Double) and get values of arguments
     * @param storageDates Where to get schedule of arguments
     *
     */
    public void values(Path path, Memoizer<TimeSeries, Double> storageValues, Memoizer<Schedule, Date> storageDates);

    /**
     * @return The arguments this node needs to return a value
     */
    public List<TimeSeries> valueChildren();
}
