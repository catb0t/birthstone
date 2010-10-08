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
#include <sstream>
#include "parser.h"

Parser::Parser(std::istream &input) : mLexer(input), mToken(Sym::NONE) {}

void Parser::run()
{
	mLexer.getNext();
	code();
}

std::string Parser::toStr  (const Token &t)
{
	if (t.getType() == Sym::STR)
		return t.getStr();
	else 	if (t.getType() == Sym::T)
		return "true";
	else 	if (t.getType() == Sym::F)
		return "false";
	else 	if (t.getType() == NUM)
	{
		std::ostringstream sstream;
		sstream << t.getNum();
		return sstream.str();
	}
	// else:
	error("toStr(): invalid type for conversion");
	return "";
}

double Parser::toNum  (const Token &t)
{
	if (t.getType() == NUM)
		return t.getNum();
	else if (t.getType() == Sym::STR)
		return atof(t.getStr().c_str());
	else 	if (t.getType() == Sym::T)
		return 1;
	else 	if (t.getType() == Sym::F)
		return 0;
	// else:
	error("toNum(): invalid type for conversion");
	return 0.0;
}
bool Parser::toBool (const Token &t)
{
	if (t.getType() == Sym::T)
		return true;
	else 	if (t.getType() == Sym::F)
		return false;
	if (t.getType() == NUM)
		return t.getNum() /*!= 0 */ ;
	else if (t.getType() == Sym::STR)
		return (t.getStr() != "");
	// else:
	error("toBool(): invalid type for conversion");
	return false;
}


inline void Parser::add(const Token &token)
{
	mTokenStk.push(token);
}

inline bool Parser::accept(Symbol sym)
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

inline bool Parser::expect(Symbol sym)
{
	if (accept(sym))
		return true;

	// else:
	error ("unexpected symbol");
	return false;
}


inline bool Parser::error(std::string msg)
{
	std::cerr << mLexer.getLine() << ": " << msg << std::endl;
	exit(1);
}




Token Parser::code()
{
	while (mToken.getType() !=  Sym::END)
		block() || ifCond() || whileLoop() || read() || print() || stmt();
}


bool Parser::print()
{
	if (accept(Sym::PRINT))
	{
		std::string str = "";
		if (stmt())
		{
			str = toStr(mTokenStk.top());
			mTokenStk.pop();
		}
		std::cout << str << std::endl;
		return true;
	}
	// else:
	return false;
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
