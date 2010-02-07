package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * @author andrea
 */
public interface TimeSeries extends Schedule
{

//    public List<Date> dates(); // from Schedule
    public void values(Path path, Memoizer storage);

    public List<TimeSeries> children();
}
