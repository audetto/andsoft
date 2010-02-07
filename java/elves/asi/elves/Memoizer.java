package asi.elves;

import asi.elves.script.*;
import java.util.*;

/**
 *
 * @author andrea
 *
 */
public class Memoizer
{
    private Map<TimeSeries, List<Double> > m_Storage = new HashMap<TimeSeries, List<Double> >();

    public void put(TimeSeries block, List<Double> values)
    {
        List<Double> constList = Collections.unmodifiableList(values);
        Object old = m_Storage.put(block, constList);
        assert old == null : block;
    }

    public List<Double> get(TimeSeries block)
    {
        return m_Storage.get(block);
    }

}
