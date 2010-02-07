package asi.elves;

import asi.elves.script.TimeSeries;
import java.util.*;

/**
 *
 * @author andrea
 */
public class ScriptEngine
{
    private TimeSeries m_root;
    private List<TimeSeries> m_allNodes;
    private List<Date> m_allDates;

    public ScriptEngine(TimeSeries root)
    {
        m_root = root;
        m_allNodes = getAllNodes();
        m_allDates = this.getAllDates(m_allNodes);
    }

    private List<TimeSeries> getAllNodes()
    {
        List<TimeSeries> allNodes = new ArrayList<TimeSeries>();
        addChildren(allNodes, m_root);
        return allNodes;
    }

    private List<Date> getAllDates(List<TimeSeries> nodes)
    {
        TreeSet<Date> dates = new TreeSet<Date>();
        for (TimeSeries node : nodes)
        {
            dates.addAll(node.dates());
        }

        List<Date> orderedDates = new ArrayList<Date>(dates);
        return orderedDates;
    }

    private void addChildren(List<TimeSeries> nodes, TimeSeries one)
    {
        if (nodes.contains(one))
            return;

        for (TimeSeries node : one.children())
        {
            addChildren(nodes, node);
        }

        nodes.add(one);
    }

    private List<Double> valuePath(Path path)
    {
        /* There is actually no need to value only the unique nodes
         * in that order.
         * We could just value the root and whenever we need the value of a child,
         * 
         * {
         *     if (!memoizer.contains(node))
         *     {
         *         memoizer.put(node, node.values(path, memoizer));
         *     }
         *     List<Double> values = memoizer.get(node);
         * }
         * 
         * For unspecified reasons, we centralize everything here.
         * It would be nice to forbit a node from calling values() directly.
         */

        Map<TimeSeries, List<Double> > memoizer = new HashMap<TimeSeries, List<Double> >();
        for (TimeSeries node : m_allNodes)
        {
            node.values(path, memoizer);
        }

        return memoizer.get(m_root);
    }
}
