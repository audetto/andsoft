/*
 *   Copyright 2004-2010 AndSoft Inc.  All Rights Reserved.
 *
 *   This file is part of the AndSoft Library.
 *   http://www.odetti.it/andrea/index.htm
 *
 *   AndSoft is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   AndSoft is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with AndSoft.  If not, see <http://www.gnu.org/licenses/>.
 */

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
