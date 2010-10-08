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
#include <cstdlib>
#include "parser.h"


Parser::Parser(std::istream &input) : mLexer(input), mToken(NONE) {}

bool Parser::error(std::string msg)
{
	std::cerr << mLexer.getLine() << msg << std::endl;
	exit(1);
}

Token Parser::code()
{
	while (token.getType() !=  Sym::END)
		block() || ifCond() || whileLoop() || read() || print() || stmt();
}


bool Parser::print()
{
	// TODO: bool Parser::print()
}

bool Parser::read()
{
	// TODO: bool Parser::read()
}


bool Parser::ifCond()
{
	// TODO: bool Parser::ifCond()
}

bool Parser::elifCond()
{
	// TODO: bool Parser::elifCond()
}

bool Parser::elseCond()
{
	// TODO: bool Parser::elseCond()
}


bool Parser::whileLoop()
{
	// TODO: bool Parser::whileLoop()
}


bool Parser::block()
{
	// TODO: bool Parser::block()
};
bool Parser::stmt()
{
	// TODO: bool Parser::stmt()
}

bool Parser::asgnmt()
{
	// TODO: bool Parser::asgnmt()
}

bool Parser::orOp()
{
	// TODO: bool Parser::orOp()
}

bool Parser::andOp()
{
	// TODO: bool Parser::andOp()
}

bool Parser::comp()
{
	// TODO: bool Parser::comp()
}

bool Parser::expr()
{
	// TODO: bool Parser::expr()
}

bool Parser::term()
{
	// TODO: bool Parser::term()
}

bool Parser::factor()
{
	// TODO: bool Parser::factor()
}
