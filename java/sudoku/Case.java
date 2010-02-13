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

import javax.swing.*;
import java.awt.*;
import java.util.*;

class Case extends JTextField
{
    private CaseInfo ci = new CaseInfo();
    private boolean fixed = false;
    private Color defaultColor;
	
    Case()
    {
		super("123456789");
		setEditable(false);
		defaultColor = getBackground();
    }
	
    void reset()
    {
		setText("123456789");
		setBackground(defaultColor);
		ci.reset();
		fixed = false;
    }
	
    boolean forbidValue(int value, Color color)
    {
		int wasFixed = ci.isFixed();

        // returns true is anything has changed
		boolean result = false;

        if (!fixed)
		{
			result = ci.forbidValue(value);
			setText(ci.toString());
		}
	    
		if (wasFixed == -1)
		{
			int isFixed = ci.isFixed();
			if (isFixed != -1)
				setBackground(color);
        }

        return result;
    }
	
    void setFixed(int value)
    {
		ci.setFixed(value);
		setText(ci.toString());
		fixed = true;
		setBackground(Color.GREEN);
    }
	
    void newFixed(int value)
    {
		int isFixed = ci.isFixed();
		if (isFixed == -1)
		{
			ci.setFixed(value);
			setText(ci.toString());
			fixed = true;
			setBackground(Color.RED);
		}
    }
	
    boolean isValid(int value)
    {
		return ci.isValid(value);
    }
	
    int isFixed()
    {
		return ci.isFixed();
    }
	
    Set<Integer> validValues()
    {
        return ci.validValues();
    }
}
