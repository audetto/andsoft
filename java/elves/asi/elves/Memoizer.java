package asi.elves;

import asi.elves.script.*;
import java.util.*;

/**
 * This class stores the partial result of the valuation of the AST
 * The actual value is List<V>
 *
 */
public class Memoizer<V>
{
    private Map<TimeSeries, List<V> > m_Storage = new HashMap<TimeSeries, List<V> >();

    /**
     * Stores values of block to be used by other blocks
     * 
     */

    public void put(TimeSeries block, List<V> values)
    {
        List<V> constList = Collections.unmodifiableList(values);
        Object old = m_Storage.put(block, constList);
        assert old == null : block;
    }

    /**
     * 
     * @return An unmodifiableList
     */

    public List<V> get(TimeSeries block)
    {
        return m_Storage.get(block);
    }

}
