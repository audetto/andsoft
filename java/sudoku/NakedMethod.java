import java.util.*;
import java.awt.*;

class NakedMethod extends SuDokuMethod
{

    NakedMethod(Case[][] guesses, Case[][] rows, Case[][] cols, Case[][] blocks)
    {
        super(guesses, rows, cols, blocks);
    }

	private void processThisBlockNaked(Case[] block, int t)
	{
		Vector<Set<Integer>> allPositions = new Vector<Set<Integer>>();
		for (Case c: block)
		{
			Set<Integer> allowedValues = c.validValues();
			allPositions.add(allowedValues);
		}

        NakedSets nakedSets = new NakedSets(allPositions, t);

		for (Set<Integer> naked : nakedSets)
		{
            boolean newNaked = false;

            // naked IS a (new ?) naked t-uple
            for (Case c: block)
            {
                Set<Integer> allowedValues = c.validValues();
                if (!naked.containsAll(allowedValues))
                {
                    for (Integer i : naked)
                    {
                        boolean changed = c.forbidValue(i, Color.PINK);
                        newNaked = newNaked | changed;
                    }
                }
            }

            // we only do 1 naked t-uple per iteration
            if (newNaked)
                return;
		}
	}

    private void goForTheNaked(int nakeds)
	{
		for (int j = 0; j < 9; ++j)
		{
			processThisBlockNaked(rows[j],	 nakeds);
			processThisBlockNaked(cols[j],	 nakeds);
			processThisBlockNaked(blocks[j], nakeds);
		}
	}

    void solve()
    {
        // naked(1) is performed by "forbidden"
        goForTheNaked(2);
        goForTheNaked(3);
        goForTheNaked(4);
    }

}
