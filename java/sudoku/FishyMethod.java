import java.util.*;
import java.awt.*;

class FishyMethod extends SuDokuMethod
{

    FishyMethod(Case[][] guesses, Case[][] rows, Case[][] cols, Case[][] blocks)
    {
        super(guesses, rows, cols, blocks);
    }

    private void processThisFishyRows(Case[][] all, int value, int t)
	{
		Vector<Set<Integer>> allPositions = new Vector<Set<Integer>>();
		for (int r = 0; r < 9; ++r)
		{
			allPositions.add(new HashSet<Integer>());
		}

		for (int r = 0; r < 9; ++r)
		{
    		for (int c = 0; c < 9; ++c)
            {
                if (all[r][c].isValid(value))
                    allPositions.elementAt(r).add(c);
            }
		}

        NakedSets nakedSets = new NakedSets(allPositions, t);

		for (Set<Integer> fishy : nakedSets)
		{
            boolean newFishy = false;

            // found (pos of a) (new?) fishy t-uple.
            for (int r = 0; r < 9; ++r)
            {
                Set<Integer> thisPositions = allPositions.elementAt(r);
                if (!fishy.containsAll(thisPositions))
                {
                    for (Integer c : fishy)
                    {
                        boolean changed = all[r][c].forbidValue(value, Color.PINK);
                        newFishy = newFishy | changed;
                    }
                }
            }

            // we only do 1 fishy t-uple per iteration
            if (newFishy)
                return;
		}
	}

    private void processThisFishyCols(Case[][] all, int value, int t)
	{
		Vector<Set<Integer>> allPositions = new Vector<Set<Integer>>();
		for (int r = 0; r < 9; ++r)
		{
			allPositions.add(new HashSet<Integer>());
		}

   		for (int c = 0; c < 9; ++c)
        {
            for (int r = 0; r < 9; ++r)
            {
                if (all[r][c].isValid(value))
                    allPositions.elementAt(c).add(r);
            }
		}

        NakedSets nakedSets = new NakedSets(allPositions, t);

		for (Set<Integer> fishy : nakedSets)
		{
            boolean newFishy = false;

            // found (pos of a) (new?) fishy t-uple.
            for (int c = 0; c < 9; ++c)
            {
                Set<Integer> thisPositions = allPositions.elementAt(c);
                if (!fishy.containsAll(thisPositions))
                {
                    for (Integer r : fishy)
                    {
                        boolean changed = all[r][c].forbidValue(value, Color.PINK);
                        newFishy = newFishy | changed;
                    }
                }
            }

            // we only do 1 fishy t-uple per iteration
            if (newFishy)
                return;
		}
	}

	private void goForTheFishy(int t)
	{
		for (int j = 0; j < 9; ++j)
		{
			processThisFishyRows(guesses, j, t);
			processThisFishyCols(guesses, j, t);
		}
	}

    void solve()
    {
        goForTheFishy(2);
        goForTheFishy(3);
        goForTheFishy(4);
    }

}
