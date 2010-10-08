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
#ifndef BS_LEXER_H
#define BS_LEXER_H

#include "symbol.h"
using Sym::Symbol;

#include <map>
#include <string>
#include <istream>

/******************************************************************************
*
******************************************************************************/
class Token
{
	public:
		
		
      Token(Symbol type = Sym::NONE);
		Token(Symbol type, const std::string &str);
		Token(Symbol type, const double &num);
		Token(const Token &token);
		~Token();

		Symbol      getType() const;
		std::string getStr()  const;
		double      getNum()  const;
		
		Token &operator =(const Token &token);

	private:
		Symbol mType;
		void *mVal;
};

/******************************************************************************
*
******************************************************************************/
class Lexer
{
	public:
		Lexer(std::istream &input);
		~Lexer();

		const Token &getNext();
		unsigned int getLine() const;

		operator bool();
	private:
		void setupKeywords();

		std::istream *mInput;
		unsigned int  mLineNum;
		Token         mToken;
		
		static std::map <std::string, Symbol> sKeywords;
};

#endif //ifndef BS_LEXER_H
