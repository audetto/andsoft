package asi.elves;

import asi.elves.script.*;
import java.util.*;

/**
 *
 * @author andrea
 */
public class ScriptEngine
{
    private TimeSeries m_root;
    private List<TimeSeries> m_allNodes;

    private List<Date> m_rootDates;
    private Memoizer<Schedule, Date> m_allDates = new Memoizer<Schedule, Date>();
    private List<Date> m_mergedDates;

    public ScriptEngine(TimeSeries root)
    {
        m_root = root;
        m_allNodes = getAllNodes();

        m_rootDates = root.checkAndStoreDates(m_allDates);

        if (m_rootDates == null)
            throw new RuntimeException("Missing dates on root node");

        m_mergedDates = mergeAllDates();
    }

    private List<TimeSeries> getAllNodes()
    {
        List<TimeSeries> allNodes = new ArrayList<TimeSeries>();
        addChildren(allNodes, m_root);
        return allNodes;
    }

    private List<Date> mergeAllDates()
    {
        TreeSet<Date> dates = new TreeSet<Date>();
        for (TimeSeries node : m_allNodes)
        {
            List<Date> nodeDates = m_allDates.get(node);

            if (nodeDates == null)
                throw new RuntimeException("Missing dates on some node");

            dates.addAll(nodeDates);
        }

        List<Date> orderedDates = new ArrayList<Date>(dates);
        return orderedDates;
    }

    private void addChildren(List<TimeSeries> nodes, TimeSeries one)
    {
        if (nodes.contains(one))
            return;

        for (TimeSeries node : one.valueChildren())
        {
            addChildren(nodes, node);
        }

        nodes.add(one);
    }

    public List<Date> allDates()
    {
        return m_mergedDates;
    }

    public void printNodeDates()
    {
        for (TimeSeries node : m_allNodes)
        {
            System.out.println(node.toString() + m_allDates.get(node).toString());
        }
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

        Memoizer<TimeSeries, Double> storage = new Memoizer<TimeSeries, Double>();
        for (TimeSeries node : m_allNodes)
        {
            node.values(path, storage, m_allDates);
        }

        return storage.get(m_root);
    }
}
