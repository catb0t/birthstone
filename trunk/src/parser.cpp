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

Parser::Parser(std::istream &input) : mLexer(input), mToken(Sym::NONE),
                                      mExec(true), mSave(NULL) {}

const Token &Parser::getNext()
{
	if (mSave)
	{
		mSave->add(mToken);
		#ifdef BS_DEBUG
			std::cerr << "saved  " << mToken << std::endl;
		#endif // BS_DEBUG
	}

	if (mTknStreams.empty())
	{
		mToken = mLexer.getNext();
		#ifdef BS_DEBUG
			std::cerr << "got " << mToken << " from Lexer"<< std::endl;
		#endif // BS_DEBUG
	}
	else
	{
		mToken = mTknStreams.top().getNext();
#ifdef BS_DEBUG
			std::cerr << "got " << mToken << " from Saved " << mTknStreams.size()<< std::endl;
#endif // BS_DEBUG
	}
	

	
	return mToken;
}

bool Parser::run()
{
	getNext();
	bool cont = true; // continue (true) or quit (false)?

	while (!accept(Sym::END) && (cont = code()))
		;
	return cont;
}

void Parser::newInput(std::istream &input)
{
 	mLexer = Lexer(input);
}


std::string Parser::toStr(Token t)
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": toStr()" << std::endl;
	t = lookup(t);
	if (t.getType() == Sym::STR)
	{
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
	error(std::string("toStr(): ") + t.repr() + " invalid type for conversion");
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
		/* DEBUG */ std::cerr << (mExec ? "accepted " : "ignored ") << mToken << std::endl;
#endif // BS_DEBUG
		 
		getNext();


	
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
	error (std::string("error: expected ") + Token(sym).repr() + " got: " + mToken.repr());
	return false;
}


inline bool Parser::error(std::string msg)
{
	std::cerr << mLexer.getLine() << ": " << msg << std::endl;
	exit(1);
}



/******************************************************************************
* returns false if quit is entered, and true otherwise
******************************************************************************/
bool Parser::code()
{
	if (accept(Sym::QUIT))
		return false;
	block() || ifCond() || loop() || print() || read() ||  stmt();
	return true;
}


bool Parser::print()
{
	bool newLine = (mToken.getType() == Sym::PRINT);

// 	std::cerr << __FILE__ << ':' << __LINE__ << ": print()" << std::endl;

	if (accept(Sym::WRITE) || accept(Sym::PRINT))
	{
// 		std::cerr << __FILE__ << ':' << __LINE__ << ": print(): got <PRINT> or <WRITE>" << std::endl;
		std::string str = "";
		if (!accept(Sym::SC))
		{
			str = toStr(asgnmt());
// 			std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
			expect(Sym::SC);
		}

		if (mExec)
		{
			std::cout << str;
			if (newLine)
				std::cout << std::endl;
		}
		return true;
	}
	// else:
	return false;
}

bool Parser::read()
{
	if (accept(Sym::READ))
	{
		Token id = mToken;
		expect(Sym::ID);

		std::map<std::string, Token>::iterator loc = mSymTbl.find(id.getStr());
		if (loc == mSymTbl.end())
		{
			std::string str;
			std::getline(std::cin, str);
			mSymTbl[id.getStr()] = Token(Sym::STR, str);
		}
		else
		{
			Symbol type = loc->second.getType();
			if (mExec)
			{
				if (type == Sym::BOOL)
				{
					double n;
					std::cin >> n;
					loc->second.setBool(n != 0);
				}
				else if (type == NUM)
				{
					double n;
					std::cin >> n;
					loc->second.setNum(n);
				}
				else if (type == Sym::STR)
				{
					std::string str;
					std::getline(std::cin, str);
					(loc->second).setStr(str);
				}
			}
		}
		expect(Sym::SC);
		return true;
	}
		
	
	return false;
}


bool Parser::ifCond()
{
	if (accept(Sym::IF))
	{
		bool condition;
		expect(Sym::O_PARAN);
		condition = toBool(asgnmt());
		condition = condition && mExec;
		expect(Sym::C_PARAN);

		bool oldExec = mExec;
		
		mExec = oldExec && condition;
		block() || stmt();
		mExec = oldExec;
		
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
	
	if (accept(Sym::ELIF))
	{
		bool condition = false;
		bool oldExec = mExec;

		expect(Sym::O_PARAN);

		if (mExec && !ignore)
			condition = toBool(asgnmt());
		else
			asgnmt();

		expect(Sym::C_PARAN);

		ignore = condition || ignore;
		mExec = oldExec && condition;
			
		block() || stmt();
		mExec = oldExec;

		return true;
	}
	// else: 
	return false;
}

bool Parser::elseCond(bool ignore)
{
	if (accept(Sym::ELSE))
	{
		bool oldExec = mExec;
		mExec = oldExec && !ignore;
		block() || stmt();
		mExec = oldExec;
		return true;
	}
	return false;
}


bool Parser::loop()
{
	bool isWhile = false, isDoWhile = false, isFor = false;
	if ((isWhile = accept(Sym::WHILE)) || (isDoWhile = accept(Sym::DO)) \
		|| (isFor = accept(Sym::FOR)))
	{
		SavedTokenStream cond; // condition
		SavedTokenStream cmds; // commands
		SavedTokenStream incr; // increment   (for loops only)
		
		bool oldExec = mExec;

		// while (...) or for(...;...;...)
		if (!isDoWhile)
		{
			expect(Sym::O_PARAN);
			if (isFor)
			{
				asgnmt();
				expect(Sym::SC);
			}
		
			mExec = false;
			mSave = &cond;
			asgnmt();
			mSave = NULL;
			cond.add(Sym::END);

			if (isFor)
			{
				expect(Sym::SC);
				mSave = &incr;
				asgnmt();
				mSave = NULL;
				incr.add(Sym::END);
			}
			expect(Sym::C_PARAN);
		}

		mExec = isDoWhile && oldExec; // do-while executes once before checking condition
		mSave = &cmds;
		block() || stmt();
		mSave = NULL;
		cmds.add(Sym::END);
		mExec = false;

		if (isDoWhile)
		{
			expect(Sym::WHILE);
			expect(Sym::O_PARAN);
			mSave = &cond;
			asgnmt();
			mSave = NULL;
			cond.add(Sym::END);
			expect(Sym::C_PARAN);
			expect(Sym::SC);
		}

		mTknStreams.push(cond);
		Token oldTkn = mToken;
		getNext();
		mExec = oldExec;

		while(toBool(asgnmt()))
		{

			Token condTkn=mToken;
			mTknStreams.push(cmds);
			getNext();
			block() || stmt();
			mTknStreams.pop(); //pop cmds
			mToken = condTkn;
			accept(Sym::END);
			if (isFor)
			{
				Token cmdsTkn=mToken;
				mTknStreams.push(incr);
				getNext();
				asgnmt();
				mTknStreams.pop(); //pop incr
				mToken = cmdsTkn;
			}

		}
		mTknStreams.pop(); //pop cond
		mToken = oldTkn;
		return true;
	}
	return false;
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

	if (print() || read())
		return true;	
	//else:
	asgnmt();
	expect(Sym::SC);

	return true;

}

Token Parser::asgnmt()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
	Token token = orOp();
	

// 	if (accept(Sym::ASSIGN))
// 	{
// 		Token token2 = lookup(orOp());
// 		if (token.getType() != Sym::ID)
// 			error("invalid lvalue in assignment");
// 		else if (mExec)
// 		{
// 			std::map<std::string, Token>::iterator loc = mSymTbl.find(token.getStr());
// 			if (loc == mSymTbl.end())
// 				error("variable not initialized");
// 			else
// 			{
// 				if (token.getType() == Sym::STR)
// 					mSymTbl[token.getStr()].setStr(toStr(token2));
// 				else if (token.getType() == Sym::NUM)
// 					mSymTbl[token.getStr()].setNum(toNum(token2));
// 				else if (token.getType() == Sym::BOOL)
// 					mSymTbl[token.getStr()].setBool(toBool(token2));
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
		else if(mExec)
			mSymTbl[token.getStr()] = token2;
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
			if (mExec)
			{
				if (token.getType() == Sym::STR)
					token = Token(Sym::STR, token.getStr() + toStr(token2));
				else if (token.getType() == Sym::NUM)
					token = Token(Sym::NUM, token.getNum() + toNum(token2));
				else if (token.getType() == Sym::BOOL)
					error("cannot add to a bool");
				mSymTbl[id] = token;
			}
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
		if (mExec)
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
		if (mExec)
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
	if (accept(Sym::EQ))
	{
		token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token.getType() == Sym::STR)
				token = Token(Sym::BOOL, token.getStr()  == toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::BOOL, token.getNum()  == toNum(token2));
			else if (token.getType() == Sym::BOOL)
				token = Token(Sym::BOOL, token.getBool() == toBool(token2));
		}
	}
	else if (accept(Sym::NOT_EQ))
	{
		token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token.getType() == Sym::STR)
				token = Token(Sym::BOOL, token.getStr()  != toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::BOOL, token.getNum()  != toNum(token2));
			else if (token.getType() == Sym::BOOL)
				token = Token(Sym::BOOL, token.getBool() != toBool(token2));
		}
	}
	else if (accept(Sym::LESS))
	{
		token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token.getType() == Sym::STR)
				token = Token(Sym::BOOL, token.getStr()  <  toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::BOOL, token.getNum()  <  toNum(token2));
			else if (token.getType() == Sym::BOOL)
				token = Token(Sym::BOOL, token.getBool() <  toBool(token2));
		}
	}
	else if (accept(Sym::LESS_EQ))
	{
		token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token.getType() == Sym::STR)
				token = Token(Sym::BOOL, token.getStr()  <= toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::BOOL, token.getNum()  <= toNum(token2));
			else if (token.getType() == Sym::BOOL)
				token = Token(Sym::BOOL, token.getBool() <= toBool(token2));
		}
	}
	else if (accept(Sym::GREATER))
	{
		token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token.getType() == Sym::STR)
				token = Token(Sym::BOOL, token.getStr()  >  toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::BOOL, token.getNum()  >  toNum(token2));
			else if (token.getType() == Sym::BOOL)
				token = Token(Sym::BOOL, token.getBool() >  toBool(token2));
		}
	}
	else if (accept(Sym::GREATER_EQ))
	{
		token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token.getType() == Sym::STR)
				token = Token(Sym::BOOL, token.getStr()  >= toStr(token2));
			else if (token.getType() == Sym::NUM)
				token = Token(Sym::BOOL, token.getNum()  >= toNum(token2));
			else if (token.getType() == Sym::BOOL)
				token = Token(Sym::BOOL, token.getBool() >= toBool(token2));
		}
	}
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
			if (mExec)
			{
				if (token.getType() == Sym::STR)
					token = Token(Sym::STR, token.getStr() + toStr(token2));
				else if (token.getType() == Sym::NUM)
					token = Token(Sym::NUM, token.getNum() + toNum(token2));
				else if (token.getType() == Sym::BOOL)
					error("cannot add to a bool");
			}
			
		}
		else if (accept(Sym::MINUS))
		{
			Token token2 = product();
			if (mExec)
			{
				if (token.getType() == Sym::STR)
					error("cannot subtract from a string");
				else if (token.getType() == Sym::NUM)
					token = Token(Sym::NUM, token.getNum() - toNum(token2));
				else if (token.getType() == Sym::BOOL)
					error("cannot subtract from a bool");
			}
		}
	}
	return token;
}

Token Parser::product()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": product()" << std::endl;
	Token token = unary();
	while ((mToken.getType() == Sym::TIMES) || (mToken.getType() == Sym::DIVIDE))
	{
		token = lookup(token);
		if (accept(Sym::TIMES))
		{
			Token token2 = unary();
			if (mExec)
			{
				if (token.getType() == Sym::STR)
					error("cannot multiply a string");
				else if (token.getType() == Sym::NUM)
					token = Token(Sym::NUM, token.getNum() * toNum(token2));
				else if (token.getType() == Sym::BOOL)
					error("cannot multiply a bool");
			}
	
		}
		else if (accept(Sym::DIVIDE))
		{
			Token token2 = unary();
			if (mExec)
			{
				if (token.getType() == Sym::STR)
					error("cannot divide string");
				else if (token.getType() == Sym::NUM)
					token = Token(Sym::NUM, token.getNum() / toNum(token2));
				else if (token.getType() == Sym::BOOL)
					error("cannot divide a bool");
			}
		}
	}
	return token;
}

// TODO: Implement unary not
Token Parser::unary()
{
	if (mExec)
	{
		if (accept(Sym::NOT))
			return Token(Sym::BOOL, !toBool(unary()));
		if (accept(Sym::MINUS))
			return Token(Sym::NUM, -toNum(unary()));
		if (accept(Sym::INCR))
		{
			Token id = mToken;
			expect(Sym::ID);
			Token &val = mSymTbl[id.getStr()];
			if (val.getType() == Sym::NUM)
			{
				val.setNum(val.getNum() + 1);
				return val;
			}
			else
				error("only numeric variables can be incremented");
		}
		if (accept(Sym::DECR))
		{
			Token id = mToken;
			expect(Sym::ID);
			Token &val = mSymTbl[id.getStr()];
			if (val.getType() == Sym::NUM)
			{
				val.setNum(val.getNum() - 1);
				return val;
			}
			else
				error("only numeric variables can be decremented");
		}
	}
	//else:
	return factor();
}

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
// 		std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
		error(std::string("got: ") + token.repr()
		      + " expected: Number, Bool, String, Identifier or parenthetical expression.");
	}
	return token;
}
