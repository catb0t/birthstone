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
#include "func.h"
#include "parser.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;
using namespace boost;




#ifdef BS_DEBUG
#include <cassert>
#endif

Parser::Parser(std::istream &input) : mLexer(input), mToken(S_NONE),
                                      mExec(true), mSave(NULL),
												  mBreak (false), mCont(false), mRet(false), mRetVal(noTkn)
{
	mSymTbls.push_back(std::map<std::string,Token>()); // global scope symbol table
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
		return boost::lexical_cast<string>(t.getNum());
	}
	else 	if (t.getType() == S_LIST)
	{	
		std::string repr = "[";
		for(std::vector<Token>::const_iterator i = t.getList().begin(); i < t.getList().end(); ++i)
		{
			if (i != t.getList().begin())
			{
				repr += ", ";
			}
			repr += toStr(*i);
		}
		repr += "]";
		return repr;
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
		return boost::lexical_cast<double>(t.getStr());
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
	else if (t.getType() == S_FILE)
		return (!t.getFile()->fail());
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
// 		error(std::string("expected identifier, got: ") + id.repr());
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
		std::cerr << "x: " << mExec << " b: " << mBreak << " c: " << mCont << '\t';
      std::cerr << (mExec ? "accepted " : "ignored ") << mToken << std::endl;
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
	error (std::string("ERROR: expected: ") + symName[sym] + " got: " + mToken.repr()
#ifdef BS_DEBUG
			+ (mTknStreams.empty()?" from Parser":" from Saved")
#endif // BS_DEBUG		
			);
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
	block() || ifCond() || loop() || defFunc() ||  stmt();
	return true;
}

bool Parser::defFunc()
{
	if (!accept(S_DEF))
		return false;
	Token id = mToken;
	expect(S_ID);
	std::string funcName = id.getStr();
	bool oldExec = mExec;
	std::vector<std::string> params;
	mExec = false;
	expect(S_O_PARAN);

	if (!accept(S_C_PARAN))
	{
		std::string param = mToken.getStr();
		if (accept(S_ID))
		{
			params.push_back(param);
			while (accept(S_COMMA))
			{
				param = mToken.getStr();
				expect(S_ID);
				params.push_back(param);
			}
		}
	
		expect(S_C_PARAN);
	}
	SavedTokenStream funcBody; // function body

	mSave = &funcBody;
	expect(S_O_BRACE);
	while (!accept(S_C_BRACE))
	{
		accept(S_RET);
		stmt();		
	}
	mSave = NULL;



	#ifdef BS_DEBUG	// DEBUG:
		std::cerr << "function defined: " << funcName << "(";
		for (std::vector<std::string>::iterator i = params.begin(); i < params.end(); ++i)
		{
			if (i != params.begin())
				std::cerr << ", ";
			std::cerr << *i; 
		}
		std::cerr << ")" << std::endl;
	#endif // BS_DEBUG

	mSymTbls.back()[funcName] = Token(Func(params,funcBody));
	// TODO: bool Parser::defFunc()
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

		if (mToken != S_SC)
		{
			if (mExec)
				str = toStr(asgnmt());
			else
				asgnmt();
		}
		if (mExec)
		{
	
			std::cout << str;
			if (newLine)
				std::cout << std::endl;
		#ifdef BS_DEBUG
			else
				std::cout << std::flush;
		#endif // BS_DEBUG
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
			if (mExec)
			{
				std::getline(std::cin, str);
			}
			mSymTbls.back()[name] = Token(str);
		}
		else
		{
			Symbol type = id.getType();
			if (mExec)
			{
				if (type == S_NUM)
				{
					string str;
					getline(std::cin, str);
					try
					{
						id.setNum(lexical_cast<double>(str));
					}
					catch (bad_lexical_cast)
					{
						id.setNum(0);
					}
				}
				else if (type == S_STR)
				{
					std::string str;
					std::getline(std::cin, str);
					id.setStr(str);
				}
				else
				{
					error("only numbers and strings can be read.");
				}
			}
		}
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
		mExec = oldExec && !(mCont || mBreak);
		
		while (elifCond(condition))
			;
		elseCond(condition);


		#ifdef BS_DEBUG
		assert( mExec == false || mCont == false);
		#endif
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
		mExec = oldExec && !(mCont || mBreak);

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
  		mExec = oldExec && !(mCont || mBreak);
		return true;
	}
	return false;
}
bool Parser::loop()
{
#warning TODO: fix do-while loop see functionalTest.bs requirement 5
	bool isWhile = false, isDoWhile = false, isFor = false, isForIn = false;
	if ((isWhile = accept(S_WHILE)) || (isDoWhile = accept(S_DO)) \
			 || (isFor = accept(S_FOR)))
	{
		SavedTokenStream cond; // condition
		SavedTokenStream cmds; // commands
		SavedTokenStream incr; // increment   (for loops only)

                // variables for for-in loops
		std::vector<Token>::const_iterator iter;
		Token id, list;
//              Sym type = S_NONE;
                
		bool oldExec = mExec;
                
                // put a new symbol table on the stack
		mSymTbls.push_back(std::map<std::string,Token>());

                // while (...) or for(...;...;...)
		if (!isDoWhile)
		{
			if (!isFor)
				expect(S_O_PARAN);
			else // if (isFor)
			{
                                //TODO: make for-in work with pre-existing variables
				if (isForIn = !accept(S_O_PARAN)) // For-in loop
				{
					isFor = false;
					id = mToken;
					expect(S_ID);
//                                      type = lookup(S_ID).getType();
					expect(S_IN);
					list = mToken;
					if (!(accept(S_ID) && (list = lookup(list)).getType() != S_LIST))
						expect(S_LIST);
					iter = list.getList().begin();

				}
				else
				{
					asgnmt();
					expect(S_SC);
				}
			}
		}

		if (!isForIn)
		{
			mExec = false;
			mSave = &cond;
			asgnmt();
			mSave = NULL;
			cond.add(S_END);
		}

		if (isFor)
		{
			expect(S_SC);
			mSave = &incr;
			asgnmt();
			mSave = NULL;
			incr.add(S_END);
		}
		expect(S_C_PARAN);
                

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
                
		mExec = oldExec;
		Token oldTkn = mToken;
		mTknStreams.push(cond);
		getNext();


		while ((isForIn &&  iter < list.getList().end()) || (!isForIn && toBool(asgnmt())))
		{
			if (isForIn)
				mSymTbls.back()[id.getStr()] = *iter;
			Token condTkn = mToken;
			mTknStreams.push(cmds);
			getNext();

			block(false) || stmt();
                        
			if (mBreak)
			{
				mBreak = false;
				mExec = true;
				mTknStreams.pop(); //pop cmds
				break;
			}
			else if (mCont)
			{
				mCont = false;
				mExec = true;
			}
			else if (!mExec)
			{
				mTknStreams.pop(); //pop cmds
				break;
			}
                        
			mTknStreams.pop(); //pop cmds
			mToken = condTkn;
			accept(S_END);
			if (isForIn)
				++iter;
			else if (isFor)
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
		mSymTbls.pop_back();
		mToken = oldTkn; //restore old Token
		return true;
	}
	return false;
}
/*

bool Parser::loop()
{
	if (accept(S_FOR))
		forLoop();
	else if (accept(S_WHILE))
		whileLoop();
	else if (accept(S_DO))
		doWhileLoop();
	else
		return false;
	return true;
}

void Parser::forLoop()
{
	if (accept(S_O_PARAN))
	{
		//...
	}
	else
		forInLoop();
}

void Parser::forInLoop()
{
	SavedTokenStream cmds;
	mSymTbls.push_back(std::map<std::string,Token>());
	
	Token id = mToken;
	expect (S_ID);
	expect(S_IN);
	Token list = asgnmt();
	if (list.getType() == S_ID)
		list = lookup(list);
	if (list.getType() != S_LIST)
		throw TypeException("List required for for-in loop");

	bool oldExec = mExec;
	mExec = false;
	mSave = &cmds;
	block(false) || stmt();
	mSave = NULL;
	cmds.add(S_END);
	mExec = oldExec;

	// 	foreach (Token t, list.getList())
	for (vector<Token>::const_iterator iter = list.getList().begin();
			 iter < list.getList().end(); ++iter)
	{
		if (mBreak)
		{
			mBreak = false;
			mExec = true;
			mTknStreams.pop(); //pop cmds
			break;
		}
		else if (mCont)
		{
			mCont = false;
			mExec = true;
		}
		else if (!mExec)
		{
			mTknStreams.pop(); //pop cmds
			break;
		}
		mSymTbls.back()[id.getStr()] = *iter; //TODO: allow for in to use pre-existing variables
		loopBody(cmds);
	}
	mSymTbls.pop_front();
}

void Parser::whileLoop()
{
	SavedTokenStream cond;
	SavedTokenStream cmds;
	bool oldExec = mExec;
	// put a new symbol table on the stack
	mSymTbls.push_back(std::map<std::string,Token>());

	expect(S_O_PARAN);
	mExec = false; //stop executing
	mSave = &cond;
	asgnmt();
	mSave = NULL;
	cond.add(S_END);
	expect(S_C_PARAN);
	
	mSave = &cmds;
	block(false) || stmt();
	mSave = NULL;
	cmds.add(S_END);
	mExec = oldExec;

	Token oldTkn = mToken;
	mTknStreams.push(cond);
	getNext();

	while (toBool(asgnmt()))
	{
		if (mBreak)
		{
			mBreak = false;
			mExec = true;
// 			mTknStreams.pop(); //pop cmds
			break;
		}
		else if (mCont)
		{
			mCont = false;
			mExec = true;
		}
		else if (!mExec)
		{
// 			mTknStreams.pop(); //pop cmds
			break;
		}
		loopBody(cmds);
	}

	mTknStreams.pop(); // pop cond
	mSymTbls.pop_front();

}


void Parser::doWhileLoop()
{
}
void Parser::loopBody(SavedTokenStream cmds)
{
	Token oldTkn = mToken;
	mTknStreams.push(cmds);
	getNext();

	block(false) || stmt();



	mTknStreams.pop(); //pop cmds
	mToken = oldTkn;
	accept(S_END);
	
	mSymTbls.pop_back();
	mToken = oldTkn; //restore old Token
		
}*/


bool Parser::block(bool createScope)
{

	if (accept(S_O_BRACE))
	{
		if (createScope)
			mSymTbls.push_back(SymTbl());
		
		while (!accept(S_C_BRACE))
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
	if (accept(S_SC))
		return false; //empty statement returns false

	if (accept(S_BREAK))
	{
		if (mExec)
		{
			mBreak = true;
			mExec = false;
		}
	}
	else if (accept(S_CONT))
	{
		if (mExec)
		{
			mCont = true;
			mExec = false;
		}
	}
	else if (accept(S_RET))
	{
		if (mExec)
		{
			mRet = true;
			if (!accept(S_SC))
			{
				mRetVal = asgnmt();
				if (mRetVal.getType() == S_ID)
					mRetVal = lookup(mRetVal);
				mExec = false;
			}
			else
			{
				mRetVal = noTkn;
				mExec = false;
				return true;
			}
			
		}
		else
			asgnmt();
	}
	else
		print() || read() || deleteCmd() || asgnmt();
	
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
					token = Token(token.getStr() + toStr(token2));
				else if (token.getType() == S_NUM)
					token = Token(token.getNum() + toNum(token2));
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
			return trueTkn;
		//else:
		return falseTkn;
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
			return Token(true);
		//else:
		return Token(false);;
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
				token = Token(token.getStr()  == toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(token.getNum()  == toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(token.getBool() == toBool(token2));
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
				token = Token(token.getStr()  != toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(token.getNum()  != toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(token.getBool() != toBool(token2));
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
				token = Token(token.getStr()  <  toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(token.getNum()  <  toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(token.getBool() <  toBool(token2));
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
				token = Token(token.getStr()  <= toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(token.getNum()  <= toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(token.getBool() <= toBool(token2));
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
				token = Token(token.getStr()  >  toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(token.getNum()  >  toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(token.getBool() >  toBool(token2));
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
				token = Token(token.getStr()  >= toStr(token2));
			else if (token.getType() == S_NUM)
				token = Token(token.getNum()  >= toNum(token2));
			else if (token.getType() == S_BOOL)
				token = Token(token.getBool() >= toBool(token2));
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
					token = Token(token.getStr() + toStr(token2));
				else if (token.getType() == S_NUM)
					token = Token(token.getNum() + toNum(token2));
				else if (token.getType() == S_BOOL)
					error("cannot add to a bool");
    				else if (token.getType() == S_LIST)
				{
					if (token2.getType() == S_ID)
						token2 = lookup(token2);
					std::vector<Token> newList(token.getList());
					if (token2.getType() == S_LIST)
					{
						for (std::vector<Token>::const_iterator i = token2.getList().begin();
							  i < token2.getList().end(); ++i)
							newList.push_back(*i);
					}
					else
						newList.push_back(token2);
					token = Token(newList);
				}
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
					token = Token(token.getNum() - toNum(token2));
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
					token = Token(token.getNum() * toNum(token2));
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
					token = Token(token.getNum() / toNum(token2));
				else if (token.getType() == S_BOOL)
					error("cannot divide a bool");
			}
		}
	}
	return token;
}

Token Parser::unary()
{

	if (accept(S_NOT))
		return (mExec?Token(!toBool(unary())):unary());
	else if (accept(S_MINUS))
		return (mExec?Token(-toNum(unary())):unary());
	else if (accept(S_INCR))
	{
		Token token = mToken;
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
		return token;
	}
	else if (accept(S_DECR))
	{
		Token token = mToken;
		accept(S_ID);
		
		if (mExec)
		{
			Token &var = lookup(token);
			if (var.getType() == S_NUM)
			{
				var.setNum(var.getNum() - 1);
				return var;
			}
			else if (var.getType() != S_NONE)
				error("only numeric variables can be decremented");
			else
				return Token(toNum(unary()));
		}
		return token;
	}
	else if (accept(S_TYPE))
	{
		Token t = unary();
		if (mExec)
		{
			if (t.getType() == S_ID)
				t = lookup(t);

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
				case S_FUNC:
					typeName = "Function";
					break;
				case S_LIST:
					typeName = "List";
					break;
				case S_FILE:
					typeName = "File";
					break;
				default:
					typeName = "Invalid";
					break;
			}
			return Token(typeName);
		}
		return t;
	}
	else if (accept(S_OPEN))
	{
		Token t = asgnmt();
		if (t.getType() == S_ID)
			t = lookup(t);
		if (t.getType() != S_STR)
			error("open requires a filename as a string");
		if (mExec)
		{
			Token file = Token(boost::shared_ptr<fstream>(new fstream(t.getStr().c_str())));
			return file;
		}
		return t;
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
	else if (accept(S_O_BRACKET))
	{
		std::vector<Token> lst;
		if (!accept(S_C_BRACKET))
		{
			Token element;
			do
			{
				element = unary();
				if (element.getType() == S_ID)
					element = lookup(element);
				lst.push_back(element);
			} while (accept(S_COMMA));
			expect(S_C_BRACKET);
		}
		token = Token(lst);
	}
	else if (accept(S_STR))
	{
		if (accept(S_F))
		{
			string formatStr = "";
			if (mExec)
			{
				Token formatStrTkn = token;
				if (formatStrTkn.getType() == S_ID)
					formatStrTkn = lookup(formatStrTkn);
				if (formatStrTkn.getType() != S_STR)
					error("format string must be a string");
				formatStr = formatStrTkn.getStr();
			}
			format f(formatStr);

	
			do {
				Token t = asgnmt();
				if (mExec)
				{
					if (t.getType() == S_ID)
						t = lookup(t);
		
					if (t.getType() == S_STR)
						f % t.getStr();
					else 	if (t.getType() == S_BOOL)
						f % t.getBool();
					else 	if (t.getType() == S_NUM)
						f % t.getNum();
					else
						error("format can only be used on numbers, strings and bools");
				}
			} while (accept(S_F));
			token =  f.str();
		}
	}
	else if (accept(S_NUM) || accept(S_BOOL) || accept(S_NONE))
		; //just accept them, we already saved the accepted token as token
	else if (accept(S_ID))
	{
		if (accept(S_O_PARAN))
		{
			return call(token);
		}
		if (accept(S_INCR) && mExec) //post-increment: old value is returned
		{
			Token &val = lookup(token);
			Token oldVal = val;
			val.setNum(val.getNum() + 1);
			return oldVal;
		}
		else if (accept(S_DECR) && mExec) //post-decrement: old value is returned
		{
			Token &val = lookup(token);
			Token oldVal = val;
			val.setNum(val.getNum() - 1);
			return oldVal;
		}

	}
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


Token Parser::call(const Token &funcName)
{
	Token &funcTkn = lookup(funcName);


	if (funcTkn.getType() == S_FUNC)
	{
		// this allows the literals to be passed by reference then destroyed when they are no longer used.
		vector<shared_ptr<Token> > literals;

		Func func = funcTkn.getFunc();
		vector<string> vec;
		

		mSymTbls.push_back(SymTbl());
		
		for (std::vector<std::string>::const_iterator i
					= func.getParams().begin();
			i < func.getParams().end(); ++i)
		{
			if (i != func.getParams().begin())
			{
				if (accept(S_C_PARAN))
					error(std::string("Too few arguments to ") + funcName.getStr());
				expect(S_COMMA);
			}
			Token arg = asgnmt();
			Token *ptr;

			if (arg.getType() == S_ID)
				ptr = &lookup(arg);
			else
			{
				literals.push_back(shared_ptr<Token>(new Token(arg)));
				ptr = literals.back().get();
			}
			Token ref(ptr);

			mSymTbls.back()[*i] = ref;
		}
		if (accept(S_COMMA))
			error(std::string("Too many arguments to ") + funcName.getStr());
		expect(S_C_PARAN);
		Token oldTkn = mToken;
		mTknStreams.push(func.getFuncBody());
		getNext();
		// TODO: implement Token Parser::call(const Token &funcName)
		block(false);
		mTknStreams.pop(); //func.getFuncBody()
		mToken = oldTkn;

		mSymTbls.pop_back();
		if (mRet)
		{
			mRet = false;
			mExec = true;
		}
		else
			mRetVal = Token();
		return mRetVal;
	}
	return noTkn; //for now
}
