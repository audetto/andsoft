package asi.elves;

import asi.elves.script.*;
import java.util.*;

/**
 * This class stores the partial result of the valuation of the AST
 * The actual value is List<V>
 *
 */
public class Memoizer<K extends Schedule, V>
{
    private Map<K, List<V> > m_Storage = new HashMap<K, List<V> >();

    /**
     * Stores values of block to be used by other blocks.
     * <p>
     * If there is already a value, an exception will be thrown if the new
     * value is not equal.
     * 
     */

    public List<V> put(K block, List<V> values)
    {
        List<V> constList = Collections.unmodifiableList(values);
        Object old = m_Storage.put(block, constList);

        if (old != null && !old.equals(values))
            throw new RuntimeException("Inconsistent values detected for " + block);

        return values;
    }

    /**
     * 
     * @return An unmodifiableList
     */

    public List<V> get(K block)
    {
        return m_Storage.get(block);
    }

}
