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

class CaseInfo
{
    private boolean[] valids = new boolean[9];
	
    CaseInfo()
    {
        reset();
    }

    void reset()
    {
        Arrays.fill(valids, true);
    }
    
    void setFixed(int value)
    {
        Arrays.fill(valids, false);
		valids[value] = true;
    }
	
    boolean forbidValue(int value)
    {
        boolean previous = valids[value];
		valids[value] = false;
        return previous;
    }
	
    boolean isValid(int value)
    {
		return valids[value];
    }

    @Override
    public String toString()
    {
		StringBuffer sb = new StringBuffer();
		boolean ok = false;
		for (int i = 0; i < 9; ++i)
		{
			if (valids[i])
			{
				sb.append(String.valueOf(i + 1));
				ok = true;
			} 
			else
			{
				sb.append(" ");
			}
		}
		
		if (ok)
			return sb.toString();
		else
			return "ERRORE";
    }
    
    int isFixed()
    {
		int count = 0;
		int value = -1;
		for (int i = 0; i < 9; ++i)
		{
			if (valids[i])
			{
				count++;
				value = i;
			}
		}
		
		if (count == 1)
			return value;
		else
			return -1;
    }
	
    Set<Integer> validValues()
    {
        Set<Integer> s = new HashSet<Integer> ();
        for (int i = 0; i < 9; ++i)
        {
            if (valids[i])
                s.add(i);
        }
		
        return s;
    }
	
}
