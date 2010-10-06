/***************************************************************************
 *   Copyright (C) 2010 by Robert Tolliver                                 *
 *   Robb.Tolli@gmail.com                                                  *
 *                                                                         *
 *   This file is part of Birthstone.                                      *
 *                                                                         *
 *   Birthstone is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *   Birthstone is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Birthstone.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/
#include "object.h"
/***********************************  Num  ***********************************/

ObjType Num::getType() const
{
	return BS_NUM;
}


/**********************************  Bool ************************************/
const char Bool::F = 0;
const char Bool::T = 1;
Bool::Bool(bool val) : mVal(val ? T : F) {}

Bool::operator bool() const { return mVal; }

ObjType Bool::getType() const
{
	return BS_BOOL;
}



/**********************************  Str  ************************************/
Str::Str (std::string str="") : mStr(str) { }
Str::operator std::string() const { return mStr; }

ObjType Str::getType() const
{
	return BS_STR;
}

