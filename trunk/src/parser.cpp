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
	mToken = mLexer.getNext();

#ifdef BS_DEBUG
	/* DEBUG */ std::cerr << "got " << mToken << std::endl;
#endif // BS_DEBUG
	
	code();
}

std::string Parser::toStr  (const Token &t)
{
	if (t.getType() == Sym::STR)
		return t.getStr();
	else 	if (t.getType() == Sym::BOOL)
		return (t.getBool() ? "true" : "false");
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
	else 	if (t.getType() == Sym::BOOL)
		return static_cast<double>(t.getBool());
	// else:
	error("toNum(): invalid type for conversion");
	return 0.0;
}
bool Parser::toBool (const Token &t)
{
	if (t.getType() == Sym::BOOL)
		return t.getBool();
	if (t.getType() == NUM)
		return t.getNum() /*!= 0 */ ;
	else if (t.getType() == Sym::STR)
		return (t.getStr() != "");
	// else:
	error("toBool(): invalid type for conversion");
	return false;
}


inline bool Parser::accept(Symbol sym)
{
	if (mToken.getType() == sym)
	{
#ifdef BS_DEBUG
		/* DEBUG */ std::cerr << "accepted " << mToken << std::endl;
#endif // BS_DEBUG
		 
		mToken = mLexer.getNext();

#ifdef BS_DEBUG
		/* DEBUG */ std::cerr << "got " << mToken << std::endl;
#endif // BS_DEBUG
	
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
// 			str = toStr(factor());
#warning "uncomment 'str = toStr(asgnmt())' and remove 'str = toStr(factor());'"
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
	return false;
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
	return false;
}

bool Parser::elseCond(bool ignore)
{
	// TODO: bool Parser::elseCond()
	return false;
}


bool Parser::whileLoop()
{
	// TODO: bool Parser::whileLoop()
	return false;
}

bool Parser::ignoreBlock()
{
	if (accept(Sym::O_BRACE))
	{
		while ((mToken.getType() != C_BRACE) && (mToken.getType() != END))
			mToken = mLexer.getNext();
		expect(Sym::C_BRACE);
		return true;
	} 
	// else:
	return false;

};

bool Parser::ignoreStmt()
{
	while ((mToken.getType() != SC) && (mToken.getType() != END))
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
	//else:
		asgnmt();
		expect(Sym::SC);
		return true;

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
	Token token = andOp();
	bool lVal = toBool(token);
	if (accept(Sym::OR))
	{
		bool rVal = toBool(asgnmt());
		lVal = lVal || rVal;

		if (lVal)
			return Token(Sym::BOOL, true);
		//else:
		return Token(Sym::BOOL, false);
	}
	//else:
	return token;
}

Token Parser::andOp()
{
	Token token = comp();
	bool lVal = toBool(token);
	if (accept(Sym::AND))
	{
		bool rVal = toBool(asgnmt());
		lVal = lVal && rVal;
		if (lVal)
			return Token(Sym::BOOL, true);
		//else:
		return Token(Sym::BOOL, false);;
		}
	//else:
	return token;
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
		Token token2 = asgnmt();
		if (token.getType() == Sym::STR)
			token = Token(Sym::STR, token.getStr() + toStr(token2));
		else if (token.getType() == Sym::NUM)
			token = Token(Sym::NUM, token.getNum() + toNum(token2));
		else if (token.getType() == Sym::BOOL)
			error("cannot add to a bool");
		
	}
	else if (accept(Sym::MINUS))
	{
		Token token2 = asgnmt();
		if (token.getType() == Sym::STR)
			error("cannot subtract from a string");
		else if (token.getType() == Sym::NUM)
			token = Token(Sym::NUM, token.getNum() + toNum(token2));
		else if (token.getType() == Sym::BOOL)
			error("cannot subtract from a bool");
	}
	return token;
}

Token Parser::product()
{
 Token token = factor();
	if (accept(Sym::TIMES))
	{
		Token token2 = asgnmt();
		if (token.getType() == Sym::STR)
			error("cannot multiply a string");
		else if (token.getType() == Sym::NUM)
			token = Token(Sym::NUM, token.getNum() * toNum(token2));
		else if (token.getType() == Sym::BOOL)
			error("cannot multiply a bool");

	}
	else if (accept(Sym::MINUS))
	{
		Token token2 = asgnmt();
		if (token.getType() == Sym::STR)
			error("cannot divide string");
		else if (token.getType() == Sym::NUM)
			token = Token(Sym::NUM, token.getNum() / toNum(token2));
		else if (token.getType() == Sym::BOOL)
			error("cannot divide a bool");
	}
	return token;
}

Token Parser::factor()
{
	
	Token token = mToken;
	if (accept(Sym::O_PARAN))
	{
		token = asgnmt();
		expect(Sym::C_PARAN);
	}
	else if ((mToken.getType() == Sym::NUM) || (mToken.getType() == Sym::BOOL) ||
		 (mToken.getType() == Sym::STR) || (mToken.getType() == Sym::ID))
		mToken = mLexer.getNext();
	else
		error("expected Number, Bool, String, Identifier or parenthetical expression.");
	return token;
}
