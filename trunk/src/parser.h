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
#include <memory>
#include "symbol.h"
#include "object.h"

using namespace Sym;
typedef std::auto_ptr P;

class SymTable
{
};

/******************************************************************************
* Birthstone parser.
******************************************************************************/
class Parser
{
	public:
		Parser(std::istream &input);
		void run();
		
	protected:
		void add(Token token);
		bool accept(Symbol sym);
		bool expect(Symbol sym);
		bool error(string msg);	
		
		P<Obj> code();

		P<Obj> ifCond();
		P<Obj> elifCond();
		P<Obj> elseCond();

		P<Obj> whileLoop();

		P<Obj> block();
		P<Obj> stmt();
		P<Obj> asgnmt();
		P<Obj> orOp();
		P<Obj> andOp();
		P<Obj> comp();
		P<Obj> expr();
		P<Obj> term();
		P<Obj> factor();
		
	private:

		Lexer mLexer;
		Token mToken;
		
};


#endif // not defined BS_PARSER_H
