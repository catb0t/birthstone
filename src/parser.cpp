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
	Token token = Sym::NONE;
	if (print()).getType()  != Sym::NONE )
		;
	else if ((token = read()).getType() != Sym::NONE)
		;
	// TODO: const Token &Parser::code()

	return token;
}


const Token &Parser::print()
{
	// TODO: const Token &Parser::print()
}

const Token &Parser::read()
{
	// TODO: const Token &Parser::read()
}


const Token &Parser::ifCond()
{
	// TODO: const Token &Parser::ifCond()
}

const Token &Parser::elifCond()
{
	// TODO: const Token &Parser::elifCond()
}

const Token &Parser::elseCond()
{
	// TODO: const Token &Parser::elseCond()
}


const Token &Parser::whileLoop()
{
	// TODO: const Token &Parser::whileLoop()
}


const Token &Parser::block()
{
	// TODO: const Token &Parser::block()
};
const Token &Parser::stmt()
{
	// TODO: const Token &Parser::stmt()
}

const Token &Parser::asgnmt()
{
	// TODO: const Token &Parser::asgnmt()
}

const Token &Parser::orOp()
{
	// TODO: const Token &Parser::orOp()
}

const Token &Parser::andOp()
{
	// TODO: const Token &Parser::andOp()
}

const Token &Parser::comp()
{
	// TODO: const Token &Parser::comp()
}

const Token &Parser::expr()
{
	// TODO: const Token &Parser::expr()
}

const Token &Parser::term()
{
	// TODO: const Token &Parser::term()
}

const Token &Parser::factor()
{
	// TODO: const Token &Parser::factor()
}