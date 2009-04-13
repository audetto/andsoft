import java.util.*;

public class NakedSets implements Iterable<Set<Integer>>
{
    private Vector<Set<Integer>> all;
    private Vector<Set<Integer>> v;
    int t;

    class NakedIterator implements Iterator<Set<Integer>>
    {
        Set<Integer> next;

        Iterator<Set<Integer>> combIter;

        NakedIterator()
        {
    		if (v.size() < t)
            {
                // no solution
                next = null;
            }
            else
            {
                combIter = new Combinatics(v.size(), t).iterator();
                next = findNext();
            }
        }

        private Set<Integer> findNext()
        {
            do
            {
                // eventually the combinations will run out
                if (!combIter.hasNext())
                    return null;

                Set<Integer> s = combIter.next();
                HashSet<Integer> naked = new HashSet<Integer>();
                for (Integer i : s)
                {
                    naked.addAll(v.elementAt(i));
                }

                int cardinality = naked.size();
                if (cardinality == s.size())
                {
                    return naked;
                }
            }
            while (true);
        }

        public boolean hasNext()
        {
            return next != null;
        }

        public Set<Integer> next()
        {
            Set<Integer> ret = next;
            next = findNext();
            return ret;
        }

        public void remove()
        {
            throw new UnsupportedOperationException();
        }
    }

    public NakedSets(Vector<Set<Integer>> all, int t)
    {
        this.all = all;
        this.t = t;

        v = new Vector<Set<Integer>>();

		for (Set<Integer> thisElement: all)
        {
            int thisSize = thisElement.size();

            // t == 1: we are looking for hidden singles
            // thisSize > t: no solutions
			if (thisSize <= t && (thisSize > 1 || t == 1))
				v.add(thisElement);
		}

    }

    public Iterator<Set<Integer>> iterator()
    {
        return new NakedIterator();
    }

}
