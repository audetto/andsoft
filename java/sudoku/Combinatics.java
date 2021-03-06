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

import java.util.*;

class Combinatics implements Iterable<Set<Integer>>
{

    class CombinationIterator implements Iterator<Set<Integer>>
    {
        private int j;
        private int x;

        private int[] c;

        CombinationIterator()
        {
            c = new int [t + 2];
            initialize();
        }

        private void initialize()
        {
            for (int i = 1; i <= t; ++i)
            {
                c[i - 1] = i - 1;
            }
            c[t]     = n;
            c[t + 1] = 0;
            j = t;
        }

        private Set<Integer> visit()
        {
            Set<Integer> s = new HashSet<Integer>();
            for (int i = 0; i < t; ++i)
                s.add(c[i]);

            return s;
        }

        public boolean hasNext()
        {
            return j <= t;
        }

        public Set<Integer> next()
        {
            // T2
            Set<Integer> s = visit();
            if (j > 0)
            {
                x = j;
                // T6
                c[j - 1] = x;
                --j;
            }
            else
            {
                // T3
                if (c[0] + 1 < c[1])
                {
                    ++c[0];
                }
                else
                {
                    j = 2;
                    // T4
                    do
                    {
                        c[j - 2] = j - 2;
                        x = c[j - 1] + 1;
                        if (x != c[j])
                            break;

                        ++j;
                    }
                    while (true);

                    // T5
                    if (j <= t)
                    {
                        // T6
                        c[j - 1] = x;
                        --j;
                    }
        		}
            }

            return s;
        }

        public void remove()
        {
            throw new UnsupportedOperationException();
        }
    }

    private int n;
    private int t;

    Combinatics(int n, int t)
    {
        if (!(n >= t && t >= 0))
            throw new IllegalArgumentException();

        this.n = n;
        this.t = t;
    }

    public Iterator<Set<Integer>> iterator()
    {
        if (n > t && t > 0)
        {
            return new CombinationIterator();
        }
        else
        {
            // The Algorithm does not work for t == n or t == 0
            // which are special cases, so we handle them here
            Vector<Set<Integer>> allCombinations = new Vector<Set<Integer>>();
            Set<Integer> all = new HashSet<Integer>();
            for (int i = 0; i < t; ++i)
            {
                all.add(i);
            }
            allCombinations.add(all);

            return allCombinations.iterator();
        }
    }

}
