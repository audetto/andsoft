package asi.elves.script;

import asi.elves.Path;
import java.util.*;

/**
 *
 * @author andrea
 */
public class Stock extends TimeSeriesBase
{
    private final String m_name;

    public Stock(String name, Schedule dates)
    {
        super(dates);
        m_name = name;
    }

    public void values(Path path, Map<TimeSeries, List<Double> > memoizer)
    {
        List<Double> stock = new ArrayList<Double>();
        for (Date date : dates())
        {
            stock.add(path.getValue(date, m_name));
        }

        memoizer.put(this, stock);
    }

}
