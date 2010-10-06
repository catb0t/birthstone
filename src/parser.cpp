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
#include <iostream>
#include "parser.h"


Parser::Parser(std::istream &input) : mLexer(input), mToken(NONE) {}

void Parser::add(Token token)
{
	// TODO: void Parser::add(Token token)
}

bool Parser::accept(Symbol sym)
{
	if (mToken.getType() == sym)
	{
		add(mToken);
		mToken == mParser.getNext();
		
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

bool Parser::error(string msg)
{
	std::cerr << lexer.getLine() << msg << endl;
}

Token Parser::code()
{
	// TODO: Token Parser::code()
}

Token Parser::ifCond()
{
	// TODO: Token Parser::ifCond()
}
Token Parser::elifCond()
{
	// TODO: Token Parser::elifCond()
}
Token Parser::elseCond()
{
	// TODO: Token Parser::elseCond()
}

Token Parser::whileLoop()
{
	// TODO: Token Parser::whileLoop()
}

Token Parser::block()
{
	// TODO: Token Parser::block()
};
Token Parser::stmt()
{
	// TODO: Token Parser::stmt()
}

Token Parser::asgnmt()
{
	// TODO: Token Parser::asgnmt()
}

Token Parser::orOp()
{
	// TODO: Token Parser::orOp()
}

Token Parser::andOp()
{
	// TODO: Token Parser::andOp()
}

Token Parser::comp()
{
	// TODO: Token Parser::comp()
}

Token Parser::expr()
{
	// TODO: Token Parser::expr()
}

Token Parser::term()
{
	// TODO: Token Parser::term()
}

Token Parser::factor()
{
	// TODO: Token Parser::factor()
}