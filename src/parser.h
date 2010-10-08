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
#ifndef BS_PARSER_H
#define BS_PARSER_H
#include <map>
#include <istream>
#include <stack>
#include <string>
#include <memory>
#include "symbol.h"
#include "lexer.h"
// #include "object.h"

using namespace Sym;


/******************************************************************************
* Birthstone parser.
******************************************************************************/
class Parser
{
	public:
		Parser(std::istream &input);
		void run();
		
	protected:
		std::string toStr  (const Token &t);
		double      toNum  (const Token &t);
		bool        toBool (const Token &t);

		void add(const Token &token);
		bool accept(Symbol sym);
		bool expect(Symbol sym);
		bool error(std::string msg);	
		
		Token code();

		bool print();
		bool read();
		
		bool ifCond();
		bool elifCond();
		bool elseCond();

		bool whileLoop();

		bool block();
		bool stmt();
		bool asgnmt();
		bool orOp();
		bool andOp();
		bool comp();
		bool expr();
		bool term();
		bool factor();


		
	private:

		Lexer mLexer;
		Token mToken;
		std::stack<Token> mTokenStk;
		std::map<std::string,Token> mSymTable;
};






#endif // not defined BS_PARSER_H
