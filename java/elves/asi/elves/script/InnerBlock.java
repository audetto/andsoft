package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * Block used by a parent block to return many results
 */
public class InnerBlock extends Function
{
    public InnerBlock(TimeSeries parent)
    {
        super(Arrays.asList(parent), MergerType.EXACT);
    }

    public void values(Path path, Memoizer<TimeSeries, Double> storage, Memoizer<Schedule, Date> storageDates)
    {
        // the parent must have added it already.
        assert storage.get(this) != null : this;
    }
}
