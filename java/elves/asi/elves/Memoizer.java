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
    private Map<K, List<V> > m_storage = new HashMap<K, List<V> >();

    /**
     * Stores values of block to be used by other blocks.
     * <p>
     * If there is already a value, an exception will be thrown if the new
     * value is not equal.
     * 
     */

    public List<V> put(K block, List<V> values)
    {
        if (values == null)
            return null;

        List<V> val = Collections.unmodifiableList(values);

        Object old = m_storage.put(block, val);

        if (old != null && !old.equals(values))
            throw new RuntimeException("Inconsistent values detected for " + block +
                    ", old: " + old + ", new: " + values);

        return values;
    }

    /**
     * 
     * @return An unmodifiableList
     */

    public List<V> get(K block)
    {
        return m_storage.get(block);
    }

}
