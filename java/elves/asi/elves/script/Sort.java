package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * @author andrea
 */
public class Sort extends TimeSeriesBase
{
    private List<TimeSeries> m_inners = new ArrayList();

    public Sort(List<TimeSeries> values)
    {
        super(values, MergerType.EXACT);
        for (TimeSeries child : children())
        {
            m_inners.add(new InnerBlock(this, this));
        }
    }

    public List<TimeSeries> innerValues()
    {
        return m_inners;
    }

    public void values(Path path, Memoizer storage)
    {
        List<Date> theDates = dates();
        int numberOfDates = theDates.size();

        List<TimeSeries> theChildren = children();
        final int numberOfValues = theChildren.size();

        // Transpose
        final List<List<Double> > allValues = new ArrayList<List<Double> >();

        for (TimeSeries child : theChildren)
        {
            List<Double> values = storage.get(child);

            // shallow copy. we MUST not modify the list returned by the memoizer.
            allValues.add(new ArrayList<Double>(values));
        }

        for (int i = 0; i < numberOfDates; ++i)
        {
            final int thisDate = i;
            
            List<Double> valuesForDate = new AbstractList<Double>()
            {

                public Double get(int index)
                {
                    return allValues.get(index).get(thisDate);
                }

                public int size()
                {
                    return numberOfValues;
                }

                @Override
                public Double set(int index, Double element)
                {
                    return allValues.get(index).set(thisDate, element);
                }

            };

            Collections.sort(valuesForDate);
        }

        for (int j = 0; j < numberOfValues; ++j)
        {
            storage.put(m_inners.get(j), allValues.get(j));
        }
    }

}
