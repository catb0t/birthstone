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

Parser::Parser(std::istream &input) : mLexer(input), mToken(S_NONE),
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

	while (!accept(S_END) && (cont = code()))
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
	if (t.getType() == S_ID)
	{
		std::string name = t.getStr();
		t = lookup(t);
		if (!t)
			error(std::string("undefined variable: ") + name);
	}
	
	if (t.getType() == S_STR)
		return t.getStr();
	else if (t.getType() == S_NONE)
		return "";
	else 	if (t.getType() == S_BOOL)
		return (t.getBool() ? "true" : "false");
	else 	if (t.getType() == S_NUM)
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
	if (t.getType() == S_ID)
	{
		std::string name = t.getStr();
		t = lookup(t);
		if (!t)
			error(std::string("undefined variable: ") + name);
	}
	
	if (t.getType() == S_NUM)
		return t.getNum();
	else if (t.getType() == S_STR)
		return atof(t.getStr().c_str());
	else 	if (t.getType() == S_BOOL)
		return static_cast<double>(t.getBool());
	// else:
	error("toNum(): invalid type for conversion");
	return 0.0;
}

bool Parser::toBool (Token t)
{
	if (t.getType() == S_ID)
	{
		std::string name = t.getStr();
		t = lookup(t);
	}


	if (t.getType() == S_BOOL)
		return t.getBool();
	else if (t.getType() == S_NONE)
		return false;
	else if (t.getType() == S_NUM)
		return t.getNum() /*!= 0 */ ;
	else if (t.getType() == S_STR)
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
	if (id.getType() != S_ID) // (token is not an ID)
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

/******************************************************************************
*	If the token is not an identifier, return the token.
*	If it is an identifier return the value of the variable.
******************************************************************************/
Token Parser::deleteVar(Token id)
{
	if (id.getType() != S_ID) // (token is not an ID)
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
		{
			symTbl->erase(loc);
			return id;
		}
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
	if (accept(S_QUIT))
		return false;
	block() || ifCond() || loop() || function() ||  stmt();
	return true;
}

bool Parser::function()
{
	if (!accept(S_DEF))
		return false;

	std::string funcName = mToken.getStr();
	expect(S_ID);
	bool oldExec = mExec;
	std::vector<Token> parameters;
	mExec = false;
	expect(S_O_PARAN);
	//....
	expect(S_C_PARAN);

	block();
	
	//TODO: bool Parser::function()
	mExec = oldExec;
	return true;
}

bool Parser::print()
{
	bool newLine = false;

// 	std::cerr << __FILE__ << ':' << __LINE__ << ": print()" << std::endl;

	if (accept(S_WRITE) || (newLine = accept(S_PRINT)))
	{
// 		std::cerr << __FILE__ << ':' << __LINE__ << ": print(): got <PRINT> or <WRITE>" << std::endl;
		std::string str = "";
		if (!accept(S_SC))
		{
			str = toStr(asgnmt());
// 			std::cerr << __FILE__ << ':' << __LINE__ << std::endl;
			expect(S_SC);
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
	if (accept(S_READ))
	{
		Token t = mToken;
		std::string name = t.getStr();
		expect(S_ID);
		Token &id = lookup(t);

		if (!id)
		{
			std::string str;
			std::getline(std::cin, str);
			mSymTbls.back()[name] = Token(S_STR, str);
		}
		else
		{
			Symbol type = id.getType();
			if (mExec)
			{
				if (type == S_BOOL)
				{
					double n;
					std::cin >> n;
					id.setBool(n != 0);
				}
				else if (type == S_NUM)
				{
					double n;
					std::cin >> n;
					id.setNum(n);
				}
				else if (type == S_STR)
				{
					std::string str;
					std::getline(std::cin, str);
					id.setStr(str);
				}
			}
		}
		expect(S_SC);
		return true;
	}
		
	
	return false;
}

bool Parser::deleteCmd()
{
	if (accept(S_DEL))
	{
		Token id = mToken;
		expect(S_ID);
		deleteVar(id);
		return true;
	}
	return false;
}

bool Parser::breakStmt() throw(Symbol)
{
	if (accept(S_BREAK))
	{
		
		if (mExec)
		{
			accept(S_SC);
			throw(S_BREAK);
		}
		return true;
	}
	return false;
}

bool Parser::contStmt() throw(Symbol)
{
	if (accept(S_CONT))
	{
		if (mExec)
		{
			accept(S_SC);
			throw(S_CONT);
		}
		return true;
	}
	return false;
}

bool Parser::ifCond()
{
	if (accept(S_IF))
	{
		bool condition;
		expect(S_O_PARAN);
		condition = toBool(asgnmt());
		condition = condition && mExec;
		expect(S_C_PARAN);

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
	
	if (accept(S_ELIF))
	{
		bool condition = false;
		bool oldExec = mExec;

		expect(S_O_PARAN);

		if (mExec && !ignore)
			condition = toBool(asgnmt());
		else
			asgnmt();

		expect(S_C_PARAN);

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
	if (accept(S_ELSE))
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
	if ((isWhile = accept(S_WHILE)) || (isDoWhile = accept(S_DO)) \
		|| (isFor = accept(S_FOR)))
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
			expect(S_O_PARAN);
			if (isFor)
			{

				asgnmt();
				expect(S_SC);
			}
		
			mExec = false;
			mSave = &cond;
			asgnmt();
			mSave = NULL;
			cond.add(S_END);

			if (isFor)
			{
				expect(S_SC);
				mSave = &incr;
				asgnmt();
				mSave = NULL;
				incr.add(S_END);
			}
			expect(S_C_PARAN);
		}

		mExec = isDoWhile && oldExec; // do-while executes once before checking condition
		mSave = &cmds;
		block() || stmt();
		mSave = NULL;
		cmds.add(S_END);
		mExec = false;

		if (isDoWhile)
		{
			expect(S_WHILE);
			expect(S_O_PARAN);
			mSave = &cond;
			asgnmt();
			mSave = NULL;
			cond.add(S_END);
			expect(S_C_PARAN);
			expect(S_SC);
		}

		mTknStreams.push(cond);
		Token oldTkn = mToken;
		getNext();
		mExec = oldExec;

		while(toBool(asgnmt()))
		{
			bool cont = false;
			Token condTkn=mToken;
			mTknStreams.push(cmds);
			getNext();
			try
			{
				block(false) || stmt();
			}
			catch (Symbol s)
			{
				if (s == S_BREAK)
					break;
				else if (s == S_CONT)
					mExec = false;
				else
					throw;
			}
			mTknStreams.pop(); //pop cmds
			mToken = condTkn;
			accept(S_END);
			if (isFor)
			{
				Token cmdsTkn=mToken;
				mTknStreams.push(incr);
				getNext();
				asgnmt();
				mTknStreams.pop(); //pop incr
				mToken = cmdsTkn;
			}
			mExec = oldExec;

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
	if (accept(S_O_BRACE))
	{
		if (createScope)
			mSymTbls.push_back(std::map<std::string,Token>());
		
		while (!accept(S_C_BRACE))
			code();
		
		if (createScope)
			mSymTbls.pop_back();

		return true;
	}
	// else:
	return false;
}

bool Parser::stmt() throw(Symbol)
{
	if (accept(S_SC))
		return false; //empty statement returns false

	if (print() || read() || deleteCmd())
		return true;

	bool breakOrCont = false;
	try
	{
		breakOrCont = breakStmt() || contStmt();
	}
	catch (Symbol)
	{
		throw;
	}
	

	if (!breakOrCont)
		asgnmt();
	expect(S_SC);

	return true;

}

Token Parser::asgnmt()
{
	Token token = orOp();

	if (accept(S_ASSIGN))
	{
		Token token2 = orOp();
		if (token2.getType() == S_ID)
			token2 = lookup(token2);
		if (token.getType() != S_ID)
			error("invalid lvalue in assignment");
		else if (mExec)
		{
			Token &var = lookup(token);
			if (!var)
				error(std::string("variable ") + token.getStr() + " must be initialized before being assigned");
			else
			{
				if (var.getType() == S_STR)
					var.setStr(toStr(token2));
				else if (var.getType() == S_NUM)
					var.setNum(toNum(token2));
				else if (var.getType() == S_BOOL)
					var.setBool(toBool(token2));
			}
		}
	}
	else if (accept(S_INIT))
	{
		Token token2 = orOp();
		if (token2.getType() == S_ID)
			token2 = lookup(token2);

		if (token.getType() != S_ID)
			error("invalid lvalue in assignment");
		else if(mExec)
			mSymTbls.back()[token.getStr()] = token2;
	}
	else if (accept(S_PLUS_EQ))
	{
		if (token.getType() != S_ID)
			error("invalid lvalue in assignment");
		else
		{
			std::string id = token.getStr();
			token = lookup(token);
			Token token2 = orOp();
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (mExec)
			{
				if (token.getType() == S_STR)
					token = Token(S_STR, token.getStr() + toStr(token2));
				else if (token.getType() == S_NUM)
					token = Token(S_NUM, token.getNum() + toNum(token2));
				else if (token.getType() == S_BOOL)
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
	while (accept(S_OR))
	{
		bool lVal = toBool(token);
		bool rVal = toBool(andOp());
		if (mExec)
			lVal = lVal || rVal;

		if (lVal)
			return Token(S_BOOL, true);
		//else:
		return Token(S_BOOL, false);
	}
	//else:
	return token;
}

Token Parser::andOp()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": andOp()" << std::endl;
	Token token = comp();
	while (accept(S_AND))
	{
		bool lVal = toBool(token);
		bool rVal = toBool(comp());
		if (mExec)
			lVal = lVal && rVal;
		if (lVal)
			return Token(S_BOOL, true);
		//else:
		return Token(S_BOOL, false);;
		}
	//else:
	return token;
}

Token Parser::comp()
{
	Token token = sum();
	if (accept(S_EQ))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (token.getType() == S_STR)
				token = Token(S_BOOL, token.getStr()  == toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(S_BOOL, token.getNum()  == toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(S_BOOL, token.getBool() == toBool(token2));
		}
	}
	else if (accept(S_NOT_EQ))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (token.getType() == S_STR)
				token = Token(S_BOOL, token.getStr()  != toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(S_BOOL, token.getNum()  != toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(S_BOOL, token.getBool() != toBool(token2));
		}
	}
	else if (accept(S_LESS))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (token.getType() == S_STR)
				token = Token(S_BOOL, token.getStr()  <  toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(S_BOOL, token.getNum()  <  toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(S_BOOL, token.getBool() <  toBool(token2));
		}
	}
	else if (accept(S_LESS_EQ))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (token.getType() == S_STR)
				token = Token(S_BOOL, token.getStr()  <= toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(S_BOOL, token.getNum()  <= toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(S_BOOL, token.getBool() <= toBool(token2));
		}
	}
	else if (accept(S_GREATER))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (token.getType() == S_STR)
				token = Token(S_BOOL, token.getStr()  >  toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(S_BOOL, token.getNum()  >  toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(S_BOOL, token.getBool() >  toBool(token2));
		}
	}
	else if (accept(S_GREATER_EQ))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		Token token2 = sum();
		if (mExec)
		{
			if (token2.getType() == S_ID)
				token2 = lookup(token2);
			if (token.getType() == S_STR)
				token = Token(S_BOOL, token.getStr()  >= toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(S_BOOL, token.getNum()  >= toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(S_BOOL, token.getBool() >= toBool(token2));
		}
	}
	return token;
}

Token Parser::sum()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": sum()" << std::endl;
	Token token = product();
	while ((mToken.getType() == S_PLUS) || (mToken.getType() == S_MINUS))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		if (accept(S_PLUS))
		{
			Token token2 = product();
			if (mExec)
			{
				if (token.getType() == S_STR)
					token = Token(S_STR, token.getStr() + toStr(token2));
				else if (token.getType() == S_NUM)
					token = Token(S_NUM, token.getNum() + toNum(token2));
				else if (token.getType() == S_BOOL)
					error("cannot add to a bool");
			}
			
		}
		else if (accept(S_MINUS))
		{
			Token token2 = product();
			if (mExec)
			{
				if (token.getType() == S_STR)
					error("cannot subtract from a string");
				else if (token.getType() == S_NUM)
					token = Token(S_NUM, token.getNum() - toNum(token2));
				else if (token.getType() == S_BOOL)
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
	while ((mToken.getType() == S_TIMES) || (mToken.getType() == S_DIVIDE))
	{
		if (token.getType() == S_ID)
			token = lookup(token);
		if (accept(S_TIMES))
		{
			Token token2 = unary();
			if (mExec)
			{
				if (token.getType() == S_STR)
					error("cannot multiply a string");
				else if (token.getType() == S_NUM)
					token = Token(S_NUM, token.getNum() * toNum(token2));
				else if (token.getType() == S_BOOL)
					error("cannot multiply a bool");
			}
	
		}
		else if (accept(S_DIVIDE))
		{
			Token token2 = unary();
			if (mExec)
			{
				if (token.getType() == S_STR)
					error("cannot divide string");
				else if (token.getType() == S_NUM)
					token = Token(S_NUM, token.getNum() / toNum(token2));
				else if (token.getType() == S_BOOL)
					error("cannot divide a bool");
			}
		}
	}
	return token;
}

// TODO: Implement unary not
Token Parser::unary()
{

	if (accept(S_NOT))
		return (mExec?Token(S_BOOL, !toBool(unary())):unary());
	else if (accept(S_MINUS))
		return (mExec?Token(S_NUM, -toNum(unary())):unary());
	else if (accept(S_INCR))
	{
		Token &var = lookup(mToken);
		expect(S_ID);
		if (mExec)
		{
			if (var.getType() == S_NUM)
			{
				var.setNum(var.getNum() + 1);
				return var;
			}
			else
				error("only numeric variables can be incremented");
		}
	}
	else if (accept(S_DECR))
	{
		Token &var = lookup(mToken);
		expect(S_ID);
		if (mExec)
		{
			if (var.getType() == S_NUM)
			{
				var.setNum(var.getNum() - 1);
				return var;
			}
			else
				error("only numeric variables can be decremented");
		}
	}
	else if (accept(S_TYPE))
	{
		bool paran = accept(S_O_PARAN);
		Token t = unary();
		if (mExec)
		{
			if (t.getType() == S_ID)
				t = lookup(t).getType();

			std::string typeName;
			switch (t.getType())
			{
				case S_NUM:
					typeName = "Number";
					break;
				case S_STR:
					typeName = "String";
					break;
				case S_BOOL:
					typeName = "Boolean";
					break;
				case S_NONE:

					typeName = "None";
					break;
				default:
					typeName = "Invalid";
					break;
			}
			if (paran)
				expect(S_C_PARAN);
			return Token(S_STR,typeName);
		}
	}
	return factor();
}

Token Parser::factor()
{
// 	std::cerr << __FILE__ << ':' << __LINE__ << ": factor()" << std::endl;
	Token token = mToken;
	if (accept(S_O_PARAN))
	{
		token = asgnmt();
		expect(S_C_PARAN);
	}
	else if (accept(S_NUM) || accept(S_BOOL) || accept(S_STR) || accept(S_ID) || accept(S_NONE))
		; //just accept them, we already saved the accepted token as token
	else
	{
		#ifdef BS_DEBUG
		/* DEBUG */ std::cerr << token << std::endl;
		#endif // BS_DEBUG

		if (token.getType() == S_FAIL)
			error(token.getStr());
		else
			error(std::string("got: ") + token.repr()
		      + " expected: Number, Bool, String, Identifier or parenthetical expression.");
	}
	return token;
}
