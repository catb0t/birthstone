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
	mStack.push(token);
}

inline bool Parser::accept(Symbol sym)
{
	if (mToken.getType() == sym)
	{
		add(mToken);
		mToken = mLexer.getNext();

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
		if (!accept(Sym::SC))
		{
			str = toStr(asgnmt());
			expect(Sym::SC);
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
	if (accept(Sym::IF))
	{
		bool condition;
		expect(Sym::O_PARAN);
		condition = toBool(asgnmt());
		expect(Sym::C_PARAN);
		
		if (condition)
		{
			block() || stmt();
		}
		else
		{
			ignoreBlock() || ignoreStmt();
			//TODO: finish bool Parser::ifCond() to call elifCond() and elseCond()
		}

		return true;
	}
	// else: 
	return false;
}

bool Parser::elifCond(bool ignore)
{
	// TODO: bool Parser::elifCond()
}

bool Parser::elseCond(bool ignore)
{
	// TODO: bool Parser::elseCond()
}


bool Parser::whileLoop()
{
	// TODO: bool Parser::whileLoop()
}

bool Parser::ignoreBlock()
{
	if (accept(Sym::O_BRACE))
	{
		while ((mToken != C_BRACE) && (mToken != END))
			mToken == mLexer.getNext();
		expect(Sym::C_BRACE);
		return true;
	} 
	// else:
	return false;

};

bool Parser::ignoreStmt()
{
	while ((mToken != SC) && (mToken != END))
		mToken = mLexer.getNext();
	expect(Sym::SC);
	return true;
}

bool Parser::block()
{
	if (accept(Sym::O_BRACE))
	{
		code();
		expect(Sym::C_BRACE);
		return true;
	}
	// else:
	return false;
}





bool Parser::stmt()
{
	if (accept(Sym::SC)) 
		return false; //empty statement returns false
	else
	{
		asgnmt();
		expect(Sym::SC);
		return true;
	}
}


Token Parser::asgnmt()
{
	Token token = orOp();
	// TODO: Token Parser::asgnmt()
// 	if (accept(Sym::ASSIGN))
// 	{
// 		
// 	}
// 	else if (accept(Sym::INIT))
// 	{
// 	}
	return token;

}

Token Parser::orOp()
{
	Bool lVal = toBool(andOp());
	if (accept(Sym::OR))
	{
		bool rVal = toBool(asgnmt());
		lVal = lVal || rVal;
	}
	if (lVal)
		return Token(Sym::T);
	//else:
	return Token(Sym::F);
}

Token Parser::andOp()
{
	Bool lVal = toBool(comp());
	if (accept(Sym::AND))
	{
		bool rVal = toBool(asgnmt());
		lVal = lVal && rVal;
	}
	if (lVal)
		return Token(Sym::T);
	//else:
	return Token(Sym::F);
}

Token Parser::comp()
{
	Token token = sum();
	// TODO: Token Parser::comp()
	return token;
}

Token Parser::sum()
{
	Token token = product();
	if (accept(Sym::PLUS))
	{
		bool token2 = toBool(asgnmt());
		token = token && token2;
	}
	// TODO: Token Parser::sum()
	return token;
}

Token Parser::product()
{
	// TODO: Token Parser::product`()
}

Token Parser::factor()
{
	// TODO: Token Parser::factor()
}