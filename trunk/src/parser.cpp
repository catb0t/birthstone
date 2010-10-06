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

#include "parser.h"

Parser::Parser(std::istream &input) : mLexer(input), mToken(NONE) {}

bool Parser::accept(Symbol sym)
{
	if (mToken.getType() == sym)
	{
		add(mToken);
		mToken == mParser.getNext();
		
		return true;
	}
	//else
	return false;
}

bool Parser::expect(Symbol sym)
{
	// TODO: bool Parser::expect()
}

bool Parser::error(string msg)
{
	// TODO: bool Parser::error(string msg)
}

P<Obj> Parser::code()
{
	// TODO: P<Obj> Parser::code()
}

P<Obj> Parser::ifCond()
{
	// TODO: P<Obj> Parser::ifCond()
}
P<Obj> Parser::elifCond()
{
	// TODO: P<Obj> Parser::elifCond()
}
P<Obj> Parser::elseCond()
{
	// TODO: P<Obj> Parser::elseCond()
}

P<Obj> Parser::whileLoop()
{
	// TODO: P<Obj> Parser::whileLoop()
}

P<Obj> Parser::block()
{
	// TODO: P<Obj> Parser::block()
};
P<Obj> Parser::stmt()
{
	// TODO: P<Obj> Parser::stmt()
}

P<Obj> Parser::asgnmt()
{
	// TODO: P<Obj> Parser::asgnmt()
}

P<Obj> Parser::orOp()
{
	// TODO: P<Obj> Parser::orOp()
}

P<Obj> Parser::andOp()
{
	// TODO: P<Obj> Parser::andOp()
}

P<Obj> Parser::comp()
{
	// TODO: P<Obj> Parser::comp()
}

P<Obj> Parser::expr()
{
	// TODO: P<Obj> Parser::expr()
}

P<Obj> Parser::term()
{
	// TODO: P<Obj> Parser::term()
}

P<Obj> Parser::factor()
{
	// TODO: P<Obj> Parser::factor()
}