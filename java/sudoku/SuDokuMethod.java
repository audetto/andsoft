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

abstract class SuDokuMethod
{
	protected Case[][] guesses;

	protected Case[][] rows;
	protected Case[][] cols;
	protected Case[][] blocks;

    SuDokuMethod(Case[][] guesses, Case[][] rows, Case[][] cols, Case[][] blocks)
    {
        this.guesses = guesses;
        this.rows = rows;
        this.cols = cols;
        this.blocks = blocks;
    }

    abstract void solve();
}
