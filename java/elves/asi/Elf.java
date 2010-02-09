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
        TimeSeries number1 = new Constant(4, null);
        TimeSeries number2 = new Constant(5, null);
        Sort sorted  = new Sort(Arrays.asList(number1, number2));
        TimeSeries first = sorted.innerValues().get(0);

        ScriptEngine engine = new ScriptEngine(first);

        System.out.println(engine.allDates());
    }
}
