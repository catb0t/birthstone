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
// typedef std::auto_ptr P;

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
		void add(const Token &token);
		bool accept(Symbol sym);
		bool expect(Symbol sym);
		bool error(std::string msg);	
		
		Token code();

		const Token &print();
		const Token &read();
		
		const Token &ifCond();
		const Token &elifCond();
		const Token &elseCond();

		const Token &whileLoop();

		const Token &block();
		const Token &stmt();
		const Token &asgnmt();
		const Token &orOp();
		const Token &andOp();
		const Token &comp();
		const Token &expr();
		const Token &term();
		const Token &factor();
		
	private:

		Lexer mLexer;
		Token mToken;
		std::stack<Token> mTokenStk;
};


inline
void Parser::run()
{
	code();
}

inline
void Parser::add(const Token &token)
{
	mTokenStk.push(token);
}

inline
bool Parser::accept(Symbol sym)
{
	if (mToken.getType() == sym)
	{
		add(mToken);
		mToken.getType() == mLexer.getNext().getType();

		return true;
	}

	// else:
	return false;
}

bool Parser::expect(Symbol sym)
{
	if (accept(sym))
		return true;

	// else:
	error ("unexpected symbol");
	return false;
}

#endif // not defined BS_PARSER_H
