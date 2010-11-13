/******************************************************************************
*   Copyright (C) 2010 by Robert Tolliver                                     *
*   Robb.Tolli@gmail.com                                                      *
*                                                                             *
*   This file is part of Birthstone.                                          *
*                                                                             *
*   Birthstone is free software: you can redistribute it and/or modify        *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation, either version 3 of the License, or         *
*  (at your option) any later version.                                        *
*                                                                             *
*   Birthstone is distributed in the hope that it will be useful,             *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*   You should have received a copy of the GNU General Public License         *
*   along with Birthstone.  If not, see <http://www.gnu.org/licenses/>.       *
*                                                                             *
******************************************************************************/
#ifndef BS_TOKEN_H
#define BS_TOKEN_H

#include <string>
#include "symbol.h"
class Func;

#include "type_exception.h"


/******************************************************************************
*
******************************************************************************/
class Token
{
	public:
		
		
      Token(Symbol type = S_NONE) throw (TypeException);
		Token(Symbol type, const std::string &str) throw (TypeException);
		Token(Symbol type, const double &num) throw (TypeException);
		Token(Symbol type, bool boolean) throw (TypeException);
		Token(Symbol type, const Func &func) throw (TypeException);
		Token(const Token &token) throw (TypeException);
		~Token();

		Symbol      getType() const;
		std::string getStr () const;
		double      getNum () const;
		bool        getBool() const;
		Func        &getFunc() const;

		void setStr(std::string s);
		void setNum(double n) ;
		void setBool(bool b);
		
		std::string repr() 	 const; // string representation of the token: "<TYPE, value>"
		
		Token &operator =(const Token &token);
		operator Symbol() const;
		operator bool() const;

	private:
		Symbol mType;
		union
		{
			double d;
			bool b;
			std::string *s;
			Func *f;
		} mVal;
};
std::ostream &operator <<(std::ostream &stream, const Token &token);

extern Token endTkn;
extern Token noTkn;
extern Token trueTkn;
extern Token falseTkn;

#endif //ndef BS_TOKEN_H
