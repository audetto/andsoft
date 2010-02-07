package asi.elves;

import asi.elves.script.*;
import java.util.*;

/**
 * This class stores the partial result of the valuation of the AST
 *
 */
public class Memoizer
{
    private Map<TimeSeries, List<Double> > m_Storage = new HashMap<TimeSeries, List<Double> >();

    /**
     * Stores values of block to be used by other blocks
     * 
     */

    public void put(TimeSeries block, List<Double> values)
    {
        List<Double> constList = Collections.unmodifiableList(values);
        Object old = m_Storage.put(block, constList);
        assert old == null : block;
    }

    /**
     * 
     * @return An unmodifiableList
     */

    public List<Double> get(TimeSeries block)
    {
        return m_Storage.get(block);
    }

}
