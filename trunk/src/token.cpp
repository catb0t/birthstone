#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <boost/any.hpp>
#include "token.h"
#include "func.h"
#include "type_exception.h"
using namespace std;
using namespace boost;

Token endTkn(S_END);
Token noTkn(S_NONE);
Token trueTkn(S_BOOL, true);
Token	falseTkn(S_BOOL, false);

Token::Token(Symbol type) throw (TypeException) : mType(type), mVal()
{
	if (mType == S_ID || mType == S_FAIL || mType == S_STR || mType == S_FUNC ||
		 mType == S_LIST)
		throw TypeException("This type requires a value");
}

Token::Token(Symbol type, const std::string &str) throw (TypeException): mType(type), mVal(str)
{
   if (!((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR)))
		throw TypeException("ERROR: only FAIL, STR, and ID types can have string values");
}

Token::Token(Symbol type, const double &num) throw (TypeException): mType(type), mVal(num)
{
   if (mType != S_NUM)
		throw TypeException("ERROR: only NUM type tokens can have numerical values");
}

Token::Token(Symbol type, bool boolean) throw (TypeException): mType(type), mVal(boolean)
{
	if (mType != S_BOOL)
		throw TypeException("ERROR: only BOOL type tokens can have boolean values");
}

Token::Token(Symbol type, const Func &func) throw (TypeException): mType(type), mVal(func)
{
	if (mType != S_FUNC)
		throw TypeException("ERROR: only FUNC type tokens can have function values");
}


Token::Token(const Token &token) throw (TypeException): mType(token.mType), mVal(token.mVal)
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
		"NONE", "FAIL", "END", "ID", "NUM", " STR", "BOOL", "FUNC",
		"O_PARAN", "C_PARAN", "O_BRACE", "C_BRACE", "O_BRACKET", "C_BRACKET",
		"PLUS_EQ", "PLUS", "MINUS", "TIMES", "DIVIDE",
		"LESS", "LESS_EQ", "EQ", "NOT_EQ", "GREATER", "GREATER_EQ",
		"IF", "ELIF", "ELSE", "WRITE", "PRINT", "DEF", "CLASS", "RET",
		"DO", "WHILE", "UNTIL", "FOR", "IN", "BREAK", "CONT", "READ", "DEL",
		"ASSIGN", "INIT", "AND", "OR", "NOT", "INCR", "DECR", "TYPE", "COMMA", "SC", "QUIT"};
		
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

Token &Token::operator =(const Token &token)
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
