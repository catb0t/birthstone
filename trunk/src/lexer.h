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
#include "token.h"
#include "token_stream.h"

#include <map>
#include <string>
#include <istream>


/******************************************************************************
*
******************************************************************************/
class Lexer : public TokenStream
{
	public:
		Lexer(std::istream &input);
		//Lexer(const Lexer &lexer);
		~Lexer();

		const Token &getNext();
		unsigned int getLine() const;
		const std::map <std::string, Token> &keywords();

		Lexer & operator=(const Lexer &lexer);
	private:
		Lexer(const Lexer &lexer); //non-copyable
		std::istream *mInput;
		unsigned int  mLineNum;
		Token         mToken;
		
};

#endif //ifndef BS_LEXER_H
