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
#include <fstream>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
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
Token::Token(shared_ptr<std::fstream> file) throw (std::bad_alloc)
	:mType(S_FILE), mVal(file) {}
Token::Token(Token *ref)throw (std::bad_alloc)
	:mType(S_REF), mVal(ref) {}
		
Token::Token(const Token &token) throw (std::bad_alloc)
	: mType(token.mType), mVal(token.mVal)
{
	
}


bool   Token::isRef()   const  throw () { return (mType == S_REF); }
inline Symbol Token::getType() const  throw () { return isRef() ? (any_cast<Token *>(mVal)->getType()) : mType; }


std::string Token::getStr() const throw (TypeException)
{
	if (isRef()) return any_cast<Token *>(mVal)->getStr();
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
	if (isRef()) return any_cast<Token *>(mVal)->getNum();
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
	if (isRef()) return any_cast<Token *>(mVal)->getBool();
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
	if (isRef()) return any_cast<Token *>(mVal)->getFunc();
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
	if (isRef()) return any_cast<Token *>(mVal)->getList();
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

shared_ptr<fstream> Token::getFile() const throw (TypeException)
{
	if (isRef()) return any_cast<Token *>(mVal)->getFile();
	if (mType == S_FILE)
	{
		try
		{
			return *any_cast<shared_ptr<fstream> >(&mVal);
		}
		catch(const boost::bad_any_cast &)
		{
			throw TypeException("ERROR: Token is not a file");
		}
	}
	else
		throw TypeException("ERROR: Token is not a file");
}



void Token::setStr(std::string s)
{
	if (isRef())
		any_cast<Token *>(mVal)->setStr(s);
	else
	{
		if ((mType != S_ID) && (mType != S_FAIL))
		{
			mType = S_STR;
		}
		mVal = s;
	}
}

void Token::setNum(double n) 
{
	if (isRef())
		any_cast<Token *>(mVal)->setNum(n);
	else
		mVal = n;
}
void Token::setRef(Token &t)
{
	mType = S_REF;
	mVal = &t;
}
Token &Token::deRef()
{
	if(isRef())
		return *any_cast<Token *>(mVal);
	return *this;
}

void Token::setBool(bool b)
{
	if (isRef())
		any_cast<Token *>(mVal)->setBool(b);
	else
		mVal = b;
}

std::string Token::repr() 	const
{
	
	std::stringstream s;
	if (isRef())
		s << "<REF ";
	s << '<';
	s << symName[getType()];

	if ((getType() == S_STR) || (getType() == S_FAIL))
		s << ", \"" << getStr() <<'"';
	else if (getType() == S_ID)
		s << ", " << getStr();
	else if (getType() == S_NUM)
		s << ", " << getNum();
	else if (getType() == S_NUM)
		s << ", " << std::boolalpha << getBool() << std::noboolalpha;
	s << '>';
	if (isRef())
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
