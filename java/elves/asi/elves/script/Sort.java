package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * Sort (pointwise) a vector of time series
 */
public class Sort extends TimeSeriesBase
{
    private List<TimeSeries> m_inners = new ArrayList<TimeSeries>();

    public Sort(List<TimeSeries> values)
    {
        super(values, MergerType.EXACT);
        for (TimeSeries child : children())
        {
            // all inner block are defined on the same dates as this
            m_inners.add(new InnerBlock(this));
        }
    }

    /**
     * The blocks used to retrieve the return values of this object from the storage.
     *
     * @return The sorted values.
     */
    public List<TimeSeries> innerValues()
    {
        return m_inners;
    }

    /**
     * This block has no value and no dates.
     * 
     * @return An empty list.
     */
    @Override
    public List<Date> dates(Memoizer<Schedule, Date> storageDates)
    {
        // empty since this block has no values
        // use the innerValues()
        List<Date> theDates = Collections.emptyList();

        return storageDates.put(this, theDates);
    }

    /**
     * This block does not provide values.
     * <p>
     * Please access results via .innerValues().
     */
    public void values(Path path, Memoizer<TimeSeries, Double> storageValues, Memoizer<Schedule, Date> storageDates)
    {
        List<Date> theDates = storageDates.get(this);
        int numberOfDates = theDates.size();

        List<TimeSeries> theChildren = children();
        final int numberOfValues = theChildren.size();

        final List<List<Double> > allValues = new ArrayList<List<Double> >();

        for (TimeSeries child : theChildren)
        {
            List<Double> values = storageValues.get(child);

            // shallow copy, since the storage returns const lists.
            allValues.add(new ArrayList<Double>(values));
        }

        for (int i = 0; i < numberOfDates; ++i)
        {
            final int thisDate = i;

            /* Anonymous class to represent a time slice (thisDate)
             * of the matrix allValues
             * Like a transpose
             */
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
            storageValues.put(m_inners.get(j), allValues.get(j));
        }

        List<Double> theValues = Collections.emptyList();
        storageValues.put(this, theValues);

    }

}
