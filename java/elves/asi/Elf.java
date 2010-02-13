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

package asi;

import java.util.*;
import asi.elves.*;
import asi.elves.script.*;

/**
 * Test class for elves
 */
public class Elf
{
    public static void main(String[] args)
    {
        Schedule dates = new ListOfDates(Arrays.asList(new Date(10, 10, 10), new Date(11, 11, 11)));

        TimeSeries number1 = new Constant(4, dates);
        TimeSeries number2 = new Constant(5);
        Sort sorted  = new Sort(Arrays.asList(number1, number2));
        TimeSeries first = sorted.sortedValues().get(0);
        TimeSeries firstDate = new Select(first, 0, 1);

        Sort sorted2  = new Sort(Arrays.asList(firstDate, number2));

        ScriptEngine engine = new ScriptEngine(sorted2.sortedValues().get(0));

        System.out.println(engine.allDates());
        engine.printNodeDates();
    }
}
