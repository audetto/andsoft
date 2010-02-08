package asi.elves.script;

import asi.elves.*;
import java.util.*;

/**
 *
 * Time series on a list of dates.
 * <p>
 * Elementary node in the AST. A list of (Date, double).
 */
public interface TimeSeries extends Schedule
{

    /**
     * @return The dates on which this block has values
     */
    public List<Date> dates(Memoizer<Schedule, Date> storageDates);

    /**
     * Store the given date for this node and all its children
     * <p>
     * Unless this node is a leaf wrt the schedule
     * <p>
     * Implementation should be
     *
     * <pre>
     * {
     *     storage.put(this, theDates);
     *     for (TimeSeries child : children())
     *     {
     *         child.forceDates(theDates, storage);
     *     }
     * }
     * </pre>
     */
   
    public void forceDates(List<Date> theDates, Memoizer<Schedule, Date> storageDates);

    /**
     * Main function to calculate the values of this block.
     * <p>
     * This function must NOT call .values() on its arguments
     * but retrieve is from the Memoizer.
     *
     * The Memoizer will store a List(Double) corresponding to the dates
     * returned by dates().
     *
     * @param path The simulated path
     * @param storage Where to store results and get values of arguments
     *
     */
    public void values(Path path, Memoizer<TimeSeries, Double> storageValues, Memoizer<Schedule, Date> storageDates);

    /**
     * @return The arguments this node needs to return a value
     */
    public List<TimeSeries> children();
}
