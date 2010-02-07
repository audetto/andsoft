package asi.elves.script;

import asi.elves.Path;
import java.util.*;

/**
 *
 * @author andrea
 */
public interface TimeSeries extends Schedule
{

//    public List<Date> dates(); // from Schedule
    public void values(Path path, Map<TimeSeries, List<Double> > memoizer);

    public List<TimeSeries> children();
}
