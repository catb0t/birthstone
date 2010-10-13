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

	while (!accept(Sym::END) && !accept(Sym::QUIT))
		code();
}

std::string Parser::toStr(Token t)
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": toStr()" << std::endl;
	t = lookup(t);
	if (t.getType() == Sym::STR)
	{
// 		std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
		return t.getStr();
	}
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

double Parser::toNum(Token t)
{
	t = lookup(t);
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

bool Parser::toBool (Token t)
{
	t = lookup(t);
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

/******************************************************************************
*	If the token is not an identifier, return the token.
*	If it is an identifier return the value of the variable.
******************************************************************************/
Token Parser::lookup(Token id)
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
	if (id.getType() != Sym::ID)
		return id;
	//else: (token is an ID)
	Token token;
	std::map<std::string, Token>::iterator loc = mSymTbl.find(id.getStr());
	if (loc == mSymTbl.end())
	{
		error(std::string("undefined variable: ") + id.getStr());
		return id;
	}
	else
		return loc->second;
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




void Parser::code()
{
		block() || ifCond() || whileLoop() || print() || read() ||  stmt();
}


bool Parser::print()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": print()" << std::endl;
	bool newLine = (mToken.getType() == Sym::PRINT);

	if (accept(Sym::WRITE) || accept(Sym::PRINT))
	{
		std::string str = "";
		if (!accept(Sym::SC))
		{
// 			std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
			str = toStr(asgnmt());
// 			std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
			expect(Sym::SC);
		}

		std::cout << str;
		if (newLine)
			std::cout << std::endl;
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
			block() || stmt();
		else
			ignoreBlock() || ignoreStmt();
		while (elifCond(condition))
			;
		elseCond(condition);


		return true;
	}
	// else: 
	return false;
}


bool Parser::elifCond(bool &ignore)
{
	
	// TODO: bool Parser::elifCond()
	if (accept(Sym::ELIF))
	{
		bool condition = false;

		expect(Sym::O_PARAN);

		if (!ignore)
			condition = toBool(asgnmt());
		else
			asgnmt();

		expect(Sym::C_PARAN);

		if (condition)
		{
			ignore = true;
			block() || stmt();
			
		}
		else
			ignoreBlock() || ignoreStmt();
		return true;
			
	}
	// else: 
	return false;
}

bool Parser::elseCond(bool ignore)
{
	if (accept(Sym::ELSE))
	{
		if (ignore)
			ignoreBlock() || ignoreStmt();
		else
			block() || stmt();
		return true;
	}
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
		while (!accept(Sym::C_BRACE))
			code();
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
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": asgnmt()" << std::endl;
	Token token = orOp();
	

// 	if (accept(Sym::ASSIGN))
// 	{
// 		if (token.getType() != Sym::ID)
// 			error("invalid lvalue in assignment");
// 		else
// 		{
// 			std::map<std::string, Token>::iterator loc = mSymTbl.find(id.getStr());
// 			if (loc == mSymTbl.end())
// 				error("variable not initialized");
// 			else
// 			{
// 				// TODO: Token Parser::asgnmt() ASSIGN
// 			}
// 		}
// 
// 	}
// 	else if (accept(Sym::INIT))
	if (accept(Sym::INIT) || accept(Sym::ASSIGN))
	{
		Token token2 = lookup(orOp());
		if (token.getType() != Sym::ID)
			error("invalid lvalue in assignment");
		else
		{
			mSymTbl[token.getStr()] = token2;
		}
	}
	else if (accept(Sym::PLUS_EQ))
	{
		if (token.getType() != Sym::ID)
			error("invalid lvalue in assignment");
		else
		{
			std::string id = token.getStr();
			token = lookup(token);
			Token token2 = lookup(orOp());
			if (token.getType() == Sym::STR)
				token = Token(Sym::STR, token.getStr() + toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::NUM, token.getNum() + toNum(token2));
			else if (token.getType() == Sym::BOOL)
				error("cannot add to a bool");
			mSymTbl[id] = token;
		}
	}

	return token;

}

Token Parser::orOp()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": orOp()" << std::endl;
	Token token = andOp();
	while (accept(Sym::OR))
	{
		bool lVal = toBool(token);
		bool rVal = toBool(andOp());
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
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": andOp()" << std::endl;
	Token token = comp();
	while (accept(Sym::AND))
	{
		bool lVal = toBool(token);
		bool rVal = toBool(comp());
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
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": sum()" << std::endl;
	Token token = product();
	while ((mToken.getType() == Sym::PLUS) || (mToken.getType() == Sym::MINUS))
	{
		token = lookup(token);
		if (accept(Sym::PLUS))
		{
			Token token2 = product();
			if (token.getType() == Sym::STR)
				token = Token(Sym::STR, token.getStr() + toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::NUM, token.getNum() + toNum(token2));
			else if (token.getType() == Sym::BOOL)
				error("cannot add to a bool");
			
		}
		else if (accept(Sym::MINUS))
		{
			Token token2 = product();
			if (token.getType() == Sym::STR)
				error("cannot subtract from a string");
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::NUM, token.getNum() + toNum(token2));
			else if (token.getType() == Sym::BOOL)
				error("cannot subtract from a bool");
		}
	}
	return token;
}

Token Parser::product()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": product()" << std::endl;
	Token token = factor();
	while ((mToken.getType() == Sym::TIMES) || (mToken.getType() == Sym::DIVIDE))
	{
		token = lookup(token);
		if (accept(Sym::TIMES))
		{
			Token token2 = factor();
			if (token.getType() == Sym::STR)
				error("cannot multiply a string");
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::NUM, token.getNum() * toNum(token2));
			else if (token.getType() == Sym::BOOL)
				error("cannot multiply a bool");
	
		}
		else if (accept(Sym::DIVIDE))
		{
			Token token2 = factor();
			if (token.getType() == Sym::STR)
				error("cannot divide string");
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::NUM, token.getNum() / toNum(token2));
			else if (token.getType() == Sym::BOOL)
				error("cannot divide a bool");
		}
	}
	return token;
}

// TODO: Implement unary not

Token Parser::factor()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": factor()" << std::endl;
	Token token = mToken;
	if (accept(Sym::O_PARAN))
	{
		token = asgnmt();
		expect(Sym::C_PARAN);
	}
	else if (accept(Sym::NUM) || accept(Sym::BOOL) || accept(Sym::STR) || accept(Sym::ID))
		; //just accept them, we already saved the accepted token as token
	else
	{
		#ifdef BS_DEBUG
		/* DEBUG */ std::cerr << token << std::endl;
		#endif // BS_DEBUG
		
		error(std::string("got: ") + token.repr()
		      + " expected: Number, Bool, String, Identifier or parenthetical expression.");
	}
	return token;
}
