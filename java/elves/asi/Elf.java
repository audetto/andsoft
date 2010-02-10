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
        TimeSeries first = sorted.innerValues().get(0);
        TimeSeries firstDate = new Select(first, 0, 1);

        Sort sorted2  = new Sort(Arrays.asList(firstDate, number2));

        ScriptEngine engine = new ScriptEngine(sorted2.innerValues().get(0));

        System.out.println(engine.allDates());
        engine.printNodeDates();
    }
}
