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
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <new>
#include <vector>
#include <boost/any.hpp>
#include "token.h"
#include "func.h"
#include "type_exception.h"
using namespace std;
using namespace boost;

Token endTkn(S_END);
Token noTkn(S_NONE);
Token trueTkn(true);
Token	falseTkn(false);

Token::Token(Symbol type) throw (TypeException)
	: mType(type), mVal()
{
	if (mType == S_ID || mType == S_FAIL || mType == S_STR || mType == S_FUNC ||
		 mType == S_LIST)
		throw TypeException("This type requires a value");
}

Token::Token(Symbol type, const std::string &str) throw (TypeException, std::bad_alloc)
	: mType(type), mVal(str)
{
   if (!((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR)))
		throw TypeException("ERROR: only FAIL, STR, and ID types can have string values");
}

Token::Token(const std::string &str)	throw (std::bad_alloc)
	: mType(S_STR), mVal(str) {}

Token::Token(const double &num) 			throw (std::bad_alloc)
	: mType(S_NUM), mVal(num) {}
Token::Token(bool boolean) 				throw (std::bad_alloc)
	: mType(S_BOOL), mVal(boolean) {}
Token::Token(const Func &func)			throw (std::bad_alloc)
	: mType(S_FUNC), mVal(func) {}
Token::Token(const std::vector<Token> &list)	throw (std::bad_alloc)
	:mType(S_LIST), mVal(list) {}

Token::Token(const Token &token) throw (std::bad_alloc)
	: mType(token.mType), mVal(token.mVal)
{
	
}

inline Symbol Token::getType() const  throw () { return mType; }


std::string Token::getStr() const throw (TypeException)
{
   if ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR))
	{
		try
		{
			return any_cast<string>(mVal);
		}
		catch(const boost::bad_any_cast &)
		{
			throw TypeException("ERROR: Token is not a string");
		}
	}
	else
		throw TypeException("ERROR: Token is not a string");
	return "";
}

double Token::getNum() const throw (TypeException)
{
   if (mType == S_NUM)
	{
		try
		{
			return any_cast<double>(mVal);
		}
		catch(const boost::bad_any_cast &)
		{
			throw TypeException("ERROR: Token is not a number");
		}
	}
	else
		throw TypeException("ERROR: Token is not a number");
	return 0.0;
}

bool Token::getBool() const throw (TypeException)
{
	if (mType == S_BOOL)
	{
		try
		{
			return any_cast<bool>(mVal);
		}
		catch(const boost::bad_any_cast &)
		{
			throw TypeException("ERROR: Token is not a boolean");
		}
	}
	else
		throw TypeException("ERROR: Token is not a boolean");
	return false;
}
		

Func Token::getFunc() const throw (TypeException)
{
	if (mType == S_FUNC)
	{
		try
		{
			return any_cast<Func>(mVal);
		}
		catch(const boost::bad_any_cast &)
		{
			throw TypeException("ERROR: Token is not a function");
		}
	}
	else
		throw TypeException("ERROR: Token is not a function");
}

const vector<Token> &Token::getList() const throw (TypeException)
{
	if (mType == S_LIST)
	{
		try
		{
			return *any_cast<vector<Token> >(&mVal);
		}
		catch(const boost::bad_any_cast &)
		{
			throw TypeException("ERROR: Token is not a list");
		}
	}
	else
		throw TypeException("ERROR: Token is not a list");
}

void Token::setStr(std::string s)
{
	if ((mType != S_ID) && (mType != S_FAIL))
	{
		mType = S_STR;
	}
	mVal = s;
}

void Token::setNum(double n) 
{
	mVal = n;
}

void Token::setBool(bool b)
{
	mVal = b;
}

std::string Token::repr() 	const
{
	std::stringstream s;
		// TODO: make sure this matches enum Symbol exactly
	static std::string typeName[] = {
	"S_NONE ", "S_FAIL", "S_END", "S_ID", "S_NUM", " S_STR", "S_BOOL", "S_LIST", "S_FUNC",
	"S_O_PARAN", "S_C_PARAN", "S_O_BRACE", "S_C_BRACE", "S_O_BRACKET", "S_C_BRACKET",
	"S_PLUS_EQ", "S_PLUS", "S_MINUS", "S_TIMES", "S_DIVIDE",
	"S_LESS", "S_LESS_EQ", "S_EQ", "S_NOT_EQ", "S_GREATER", "S_GREATER_EQ",
	"S_IF", "S_ELIF", "S_ELSE",
	"S_WRITE", "S_PRINT", "S_READ", "S_DEF", "S_CLASS", "S_RET", "S_DEL",
	"S_DO", "S_WHILE", "S_UNTIL", "S_FOR", "S_IN", "S_BREAK", "S_CONT",
	"S_ASSIGN", "S_INIT", "S_AND", "S_OR", "S_NOT", "S_INCR", "S_DECR", "S_TYPE", "S_COMMA", "S_SC", "S_QUIT"
		
	};
		
		s << '<';
// 		if ((getType() >= S_NONE) && (getType() <= S_QUIT))
			 s << typeName[getType()];
// 		else
// 			s << getType();

		if ((getType() == S_STR) || (getType() == S_FAIL))
			s << ", \"" << getStr() <<'"';
		else if (getType() == S_ID)
			s << ", " << getStr();
		else if (getType() == S_NUM)
			s << ", " << getNum();
		else if (getType() == S_NUM)
			s << ", " << std::boolalpha << getBool() << std::noboolalpha;
		s << '>';
		return s.str();
}

Token &Token::operator =(const Token &token) throw(std::bad_alloc)
{

	mType = token.mType;
	mVal = token.mVal;
	return *this;
}

Token::operator Symbol() const
{
	return getType();
}

Token::operator bool() const
{
	return (getType() != S_NONE);
}

std::ostream &operator <<(std::ostream &stream, const Token &token)
{

	stream << token.repr();
	return stream;
}
