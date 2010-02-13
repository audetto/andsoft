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
 * This class represents a List of Dates
 */
public interface Schedule
{
    public List<Date> checkAndStoreDates(Memoizer<Schedule, Date> storageDates);

    public List<? extends Schedule> dateChildren();

    /**
     * Store the given date for this node and all its children
     * <p>
     * Unless this node is a leaf wrt the schedule
     */

    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates);
}
