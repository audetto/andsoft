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

class HiddenMethod extends SuDokuMethod
{

    HiddenMethod(Case[][] guesses, Case[][] rows, Case[][] cols, Case[][] blocks)
    {
        super(guesses, rows, cols, blocks);
    }

    private void processThisBlockHidden(Case[] block, int t)
	{
		Vector<Set<Integer>> allPositions = new Vector<Set<Integer>>();
		for (int i = 0; i < 9; ++i)
		{
			allPositions.add(new HashSet<Integer>());
		}

		for (int i = 0; i < 9; ++i)
		{
			Set<Integer> allowedValues = block[i].validValues();
            for (Integer j : allowedValues)
            {
                allPositions.elementAt(j).add(i);
            }
		}

        NakedSets nakedSets = new NakedSets(allPositions, t);

		for (Set<Integer> hidden : nakedSets)
		{
            boolean newHidden = false;

            // found (pos of a) (new?) hidden t-uple.
            for (int i = 0; i < 9; ++i)
            {
                Set<Integer> thisPositions = allPositions.elementAt(i);
                if (!hidden.containsAll(thisPositions))
                {
                    for (Integer noPos : hidden)
                    {
                        boolean changed = block[noPos].forbidValue(i, Color.PINK);
                        newHidden = newHidden | changed;
                    }
                }
            }

            // we only do 1 hidden t-uple per iteration
            if (newHidden)
                return;
		}
	}

    private void goForTheHidden(int hiddens)
	{
		for (int j = 0; j < 9; ++j)
		{
			processThisBlockHidden(rows[j],	  hiddens);
			processThisBlockHidden(cols[j],	  hiddens);
			processThisBlockHidden(blocks[j], hiddens);
		}
	}

    void solve()
    {
        goForTheHidden(1);
        goForTheHidden(2);
        goForTheHidden(3);
        goForTheHidden(4);
    }

}
