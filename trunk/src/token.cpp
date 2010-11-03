#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include "token.h"
#include "func.h"

Token endTkn(S_END);
Token noTkn(S_NONE);
Token trueTkn(S_BOOL, true);
Token	falseTkn(S_BOOL, false);

Token::Token(Symbol type) : mType(type)
{
	if ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR))
		mVal.s = new std::string("");
}

Token::Token(Symbol type, const std::string &str) : mType(type)
{
   if ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR))
		mVal.s = new std::string(str);
	else
	{
      mType = S_FAIL;
		std::string error = "ERROR: only FAIL, STR, and ID types can have string values";
		mVal.s = new std::string(error);
	}
}

Token::Token(Symbol type, const double &num) : mType(type)
{
   if (mType == S_NUM)
		mVal.d =num;
	else
	{
      mType = S_FAIL;
		std::string error = "ERROR: only NUM type tokens can have numerical values";
		mVal.s = new std::string(error);
	}
}

Token::Token(Symbol type, bool boolean) : mType(type)
{
	if (mType == S_BOOL)
		mVal.b = boolean;
	else
	{
		mType = S_FAIL;
		std::string error = "ERROR: only BOOL type tokens can have boolean values";
		mVal.s = new std::string(error);
	}
}

Token::Token(Symbol type, const Func &func)
{
#warning TODO: implement Token::Token(Symbol type, const Func &func)
}


Token::Token(const Token &token) : mType(token.mType)
{
      if ((token.mType == S_ID) || (token.mType == S_FAIL) || (token.mType == S_STR))
			mVal.s = new std::string(*token.mVal.s);
      else if (token.mType == S_NUM)
			mVal.d = token.mVal.d;
		else if (token.mType == S_BOOL)
			mVal.b = token.mVal.b;
}

Token::~Token()
{
	if ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR))
	{
		if (mVal.s)
			delete  mVal.s;
		mVal.s = NULL;
	}
}

inline Symbol Token::getType() const { return mType; }


std::string Token::getStr() const
{
   if (mVal.s && ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR)))
		return *mVal.s;
	else
		return "";
}

double Token::getNum() const
{
   if (mType == S_NUM)
		return mVal.d;
	else
		return 0.0;
}

bool Token::getBool() const
{
	if (mType == S_BOOL)
		return mVal.b;
	else
		return false;
}
		

Func Token::getFunc() const
{
#warning TODO: implement Func Token::getFunc() const
}
		
void Token::setStr(std::string s)
{
	if (mVal.s && ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR)))
	{
		delete mVal.s;
		mVal.s = new std::string(s);
	}
	else
	{
		mType = S_STR;
		mVal.s = new std::string(s);
	}
}

void Token::setNum(double n) 
{
	if (mVal.s && ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR)))
		delete [] mVal.s;
	mType = S_NUM;
	mVal.d = n;
}

void Token::setBool(bool b)
{
	if (mVal.s && ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR)))
		delete [] mVal.s;
	mType = S_BOOL;
	mVal.b = b;
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

	if ((mType == S_ID) || (mType == S_FAIL) || (mType == S_STR))
	{
		if (mVal.s)
			delete  mVal.s;
		mVal.s = NULL;
	}
	mType = token.mType;
	
	if ((token.mType == S_ID) || (token.mType == S_FAIL) || (token.mType == S_STR))
		mVal.s = new std::string(*token.mVal.s);
	else if (token.mType == S_NUM)
		mVal.d = token.mVal.d;
	else if (token.mType == S_BOOL)
		mVal.b = token.mVal.b;
	
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
