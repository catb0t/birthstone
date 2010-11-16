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
#include <new>
#include <vector>
#include <boost/any.hpp>
#include "symbol.h"
class Func;

#include "type_exception.h"


/******************************************************************************
*
******************************************************************************/
class Token
{
	public:
      Token(Symbol type = S_NONE) 						throw (TypeException);
		Token(Symbol type, const std::string &str) 	throw (TypeException, std::bad_alloc);

		Token(const std::string &str)				throw (std::bad_alloc); // Type: S_STR
		Token(const double &num) 					throw (std::bad_alloc); // Type: S_NUM
		Token(bool boolean) 							throw (std::bad_alloc); // Type: S_BOOL
		Token(const Func &func)						throw (std::bad_alloc); // Type: S_FUNC
		Token(const std::vector<Token> &list)	throw (std::bad_alloc); // Type: S_LIST
		
		Token(const Token &token) throw (std::bad_alloc);

		Symbol      getType() const throw ();
		std::string getStr () const throw (TypeException);
		double      getNum () const throw (TypeException);
		bool        getBool() const throw (TypeException);
		Func        getFunc() const throw (TypeException);
		const std::vector<Token> &getList() const throw (TypeException);

		void setStr(std::string s);
		void setNum(double n) ;
		void setBool(bool b);
		
		std::string repr() 	 const; // string representation of the token: "<TYPE, value>"
		
		Token &operator =(const Token &token) throw(std::bad_alloc);
		operator Symbol() const;
		operator bool() const;

	private:
 		Symbol mType;
 		boost::any mVal;
};
std::ostream &operator <<(std::ostream &stream, const Token &token);

extern Token endTkn;
extern Token noTkn;
extern Token trueTkn;
extern Token falseTkn;

#endif //ndef BS_TOKEN_H