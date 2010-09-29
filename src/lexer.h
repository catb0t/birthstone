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
#include <map>
#include <string>
#include <istream>
#include <ostream>
#include <cctype>
#include <cstdlib>
/******************************************************************************
*
******************************************************************************/
class Token
{
	public:
		enum TokenType {NONE = 0, FAIL, END, ID, NUM,  STR, T, F,
			O_PARAN, C_PARAN,O_BRACE, C_BRACE, O_BRACKET, C_BRACKET,
			PLUS_EQ, PLUS, MINUS, TIMES, DIVIDE,
			LESS, LESS_EQ, EQ, NOT_EQ, GREATER, GREATER_EQ,
			IF, ELIF, ELSE, PRINT, DEF, CLASS,
			DO, WHILE, UNTIL, FOR, IN, BREAK, CONT, READ,
			ASSIGN, INIT, AND, OR, NOT, COMMA, SC};
		
		Token(TokenType type = NONE);
		Token(TokenType type, const std::string &str);
		Token(TokenType type, const double &num);
		Token(const Token &token);
		~Token();

		TokenType   getType() const;
		std::string getStr()  const;
		double      getNum()  const;
		Token &operator =(const Token &token);

	private:
		TokenType mType;
		void *mVal;
};

/******************************************************************************
*
******************************************************************************/
class Lexer
{
	public:
		Lexer(std::istream *input);
		Lexer(std::istream &input);
		~Lexer();

		const Token &getNext();
	private:
		void setupKeywords();

		std::istream *mInput;
		unsigned int  mLineNum;
		Token         mToken;
		
		static std::map <std::string, Token::TokenType> sKeywords;
};
std::map <std::string, Token::TokenType> Lexer::sKeywords;

#endif //ifndef BS_LEXER_H
