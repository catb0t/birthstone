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
#ifndef BS_OBJECT_H
#define BS_OBJECT_H
#include <string>
enum Type {BS_STR, BS_NUM, BS_BOOL};

class Object
{
	public:
		virtual Type getType() = 0;
	protected:

	private:	
};


class Number : public Object
{
	public:
		Number(double num = 0.0);
		Type getType() const;
		operator double() const;
	private:
		double mNum;
}; 

class Boolean : public Object
{
	public:
		static const char T;
		static const char F;
		
		Boolean(bool val = false);
		
		Type getType() const;
		operator bool() const;
		
	private:
		char mVal;
	
};

class String : public Object
{
	public:
		
}

#endif // BS_OBJECT_H
