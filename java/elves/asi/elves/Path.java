package asi.elves;

import java.util.*;

/**
 *
 * The simulated path
 */
public interface Path
{
    /**
     * Value of a stock on a given date.
     * <p>
     * This is the main access to the stochastic path.
     *
     */
    public double getValue(Date date, int id);

}
