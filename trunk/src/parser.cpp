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
                                      mExec(true), mSave(NULL)
{
	mSymTbls.push_back(std::map<std::string,Token>());
}

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
	if (t.getType() == Sym::ID)
	{
		std::string name = t.getStr();
		t = lookup(t);
		if (!t)
			error(std::string("undefined variable: ") + name);
	}
	
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
	if (t.getType() == Sym::ID)
	{
		std::string name = t.getStr();
		t = lookup(t);
		if (!t)
			error(std::string("undefined variable: ") + name);
	}
	
	if (t.getType() == Sym::NUM)
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
	if (t.getType() == Sym::ID)
	{
		std::string name = t.getStr();
		t = lookup(t);
		if (!t)
			error(std::string("undefined variable: ") + name);
	}

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
Token &Parser::lookup(Token id)
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
	if (id.getType() != Sym::ID) // (token is not an ID)
	{
		error(std::string("expected identifier, got: ") + id.repr());
		return noTkn;  
	}
	//else: (token is an ID)
	std::string name = id.getStr();
	for (std::list<std::map<std::string,Token> >::reverse_iterator symTbl = mSymTbls.rbegin();
			 symTbl != mSymTbls.rend(); ++symTbl)
	{
		std::map<std::string, Token>::iterator loc = symTbl->find(name);
		if (loc != symTbl->end())
			return loc->second;
	}
	return noTkn;
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
	block() || ifCond() || loop() || function() ||  stmt();
	return true;
}

bool Parser::function()
{
	if (!accept(Sym::DEF))
		return false;

	std::string funcName = mToken.getStr();
	expect(Sym::ID);
	bool oldExec = mExec;
	std::vector<Token> parameters;
	mExec = false;
	expect(Sym::O_PARAN);
	//....
	expect(Sym::C_PARAN);

	block();
	
	//TODO: bool Parser::function()
	mExec = oldExec;
	return true;
}

bool Parser::print()
{
	bool newLine;

// 	std::cerr << __FILE__ << ':' << __LINE__ << ": print()" << std::endl;

	if (accept(Sym::WRITE) || (newLine = accept(Sym::PRINT)))
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
		Token t = mToken;
		std::string name = t.getStr();
		expect(Sym::ID);
		Token &id = lookup(t);

		if (!id)
		{
			std::string str;
			std::getline(std::cin, str);
			mSymTbls.back()[name] = Token(Sym::STR, str);
		}
		else
		{
			Symbol type = id.getType();
			if (mExec)
			{
				if (type == Sym::BOOL)
				{
					double n;
					std::cin >> n;
					id.setBool(n != 0);
				}
				else if (type == Sym::NUM)
				{
					double n;
					std::cin >> n;
					id.setNum(n);
				}
				else if (type == Sym::STR)
				{
					std::string str;
					std::getline(std::cin, str);
					id.setStr(str);
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
		
		// put a new symbol table on the stack
		mSymTbls.push_back(std::map<std::string,Token>());

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
			block(false) || stmt();
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
		mSymTbls.pop_back();
		return true;
	}
	return false;
}

bool Parser::block(bool createScope)
{
	if (accept(Sym::O_BRACE))
	{
		if (createScope)
			mSymTbls.push_back(std::map<std::string,Token>());
		
		while (!accept(Sym::C_BRACE))
			code();
		
		if (createScope)
			mSymTbls.pop_back();

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
	

	if (accept(Sym::ASSIGN))
	{
		Token token2 = orOp();
		if (token2.getType() == Sym::ID)
			token2 = lookup(token2);
		if (token.getType() != Sym::ID)
			error("invalid lvalue in assignment");
		else if (mExec)
		{
			Token &var = lookup(token);
			if (!var)
				error(std::string("variable ") + token.getStr() + " must be initialized before being assigned");
			else
			{
				if (var.getType() == Sym::STR)
					var.setStr(toStr(token2));
				else if (var.getType() == Sym::NUM)
					var.setNum(toNum(token2));
				else if (var.getType() == Sym::BOOL)
					var.setBool(toBool(token2));
			}
		}

	}
	else if (accept(Sym::INIT))
// 	if (accept(Sym::INIT) || accept(Sym::ASSIGN))
	{
		Token token2 = orOp();
		if (token2.getType() == Sym::ID)
			token2 = lookup(token2);
		if (token.getType() != Sym::ID)
			error("invalid lvalue in assignment");
		else if(mExec)
			mSymTbls.back()[token.getStr()] = token2;
	}
	else if (accept(Sym::PLUS_EQ))
	{
		if (token.getType() != Sym::ID)
			error("invalid lvalue in assignment");
		else
		{
			std::string id = token.getStr();
			token = lookup(token);
			Token token2 = orOp();
			if (token2.getType() == Sym::ID)
				token2 = lookup(token2);
			if (mExec)
			{
				if (token.getType() == Sym::STR)
					token = Token(Sym::STR, token.getStr() + toStr(token2));
				else if (token.getType() == Sym::NUM)
					token = Token(Sym::NUM, token.getNum() + toNum(token2));
				else if (token.getType() == Sym::BOOL)
					error("cannot add to a bool");
				mSymTbls.back()[id] = token;
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
		if (token.getType() == Sym::ID)
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
		if (token.getType() == Sym::ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == Sym::ID)
				token2 = lookup(token);
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
		if (token.getType() == Sym::ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == Sym::ID)
				token2 = lookup(token);
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
		if (token.getType() == Sym::ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == Sym::ID)
				token2 = lookup(token);
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
		if (token.getType() == Sym::ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == Sym::ID)
				token2 = lookup(token);
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
		if (token.getType() == Sym::ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == Sym::ID)
				token2 = lookup(token);
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
		if (token.getType() == Sym::ID)
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
		if (token.getType() == Sym::ID)
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
		else if (accept(Sym::MINUS))
			return Token(Sym::NUM, -toNum(unary()));
		else if (accept(Sym::INCR))
		{
			Token &var = lookup(mToken);
			expect(Sym::ID);
			if (var.getType() == Sym::NUM)
			{
				var.setNum(var.getNum() + 1);
				return var;
			}
			else
				error("only numeric variables can be incremented");
		}
		else if (accept(Sym::DECR))
		{
			Token &var = lookup(mToken);
			expect(Sym::ID);
			if (var.getType() == Sym::NUM)
			{
				var.setNum(var.getNum() - 1);
				return var;
			}
			else
				error("only numeric variables can be decremented");
		}
		else if (accept(Sym::TYPE))
		{
			Token t = mToken;
			expect(Sym::ID);
			bool paran = accept(Sym::O_PARAN);
			std::string typeName;
			switch (lookup(t).getType())
			{
				case Sym::NUM:
					typeName = "Number";
					break;
				case Sym::STR:
					typeName = "String";
					break;
				case Sym::BOOL:
					typeName = "Boolean";
					break;
				default:
					typeName = "None";
					break;
			}
			if (paran)
				expect(Sym::C_PARAN);
			return Token(Sym::STR,typeName);
		}
	}
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
