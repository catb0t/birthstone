#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include "token.h"




Token::Token(Symbol type) : mType(type)
{
	if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
		mVal.s = new std::string("");
}

Token::Token(Symbol type, const std::string &str) : mType(type)
{
   if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
		mVal.s = new std::string(str);
	else
	{
      mType = Sym::FAIL;
		std::string error = "ERROR: only FAIL, STR, and ID types can have string values";
		mVal.s = new std::string(error);
	}
}

Token::Token(Symbol type, const double &num) : mType(type)
{
   if (mType == Sym::NUM)
		mVal.d =num;
	else
	{
      mType = Sym::FAIL;
		std::string error = "ERROR: only NUM type tokens can have numerical values";
		mVal.s = new std::string(error);
	}
}

Token::Token(Symbol type, bool boolean) : mType(type)
{
	if (mType == Sym::BOOL)
		mVal.b = boolean;
	else
	{
		mType = Sym::FAIL;
		std::string error = "ERROR: only BOOL type tokens can have boolean values";
		mVal.s = new std::string(error);
	}
}

Token::Token(const Token &token) : mType(token.mType)
{
      if ((token.mType == Sym::ID) || (token.mType == Sym::FAIL) || (token.mType == Sym::STR))
			mVal.s = new std::string(*token.mVal.s);
      else if (token.mType == Sym::NUM)
			mVal.d = token.mVal.d;
		else if (token.mType == Sym::BOOL)
			mVal.b = token.mVal.b;
}

Token::~Token()
{
	if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
	{
		if (mVal.s)
			delete  mVal.s;
		mVal.s = NULL;
	}
}

inline Symbol Token::getType() const { return mType; }


std::string Token::getStr() const
{
   if (mVal.s && ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR)))
		return *mVal.s;
	else
		return "";
}

double Token::getNum() const
{
   if (mType == Sym::NUM)
		return mVal.d;
	else
		return 0.0;
}

bool Token::getBool() const
{
	if (mType == Sym::BOOL)
		return mVal.b;
	else
		return false;
}

				
void Token::setStr(std::string s)
{
	if (mVal.s && ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR)))
	{
		delete mVal.s;
		mVal.s = new std::string(s);
	}
	else
	{
		mType = Sym::STR;
		mVal.s = new std::string(s);
	}
}

void Token::setNum(double n) 
{
	if (mVal.s && ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR)))
		delete [] mVal.s;
	mType = Sym::NUM;
	mVal.d = n;
}

void Token::setBool(bool b)
{
	if (mVal.s && ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR)))
		delete [] mVal.s;
	mType = Sym::BOOL;
	mVal.b = b;
}

std::string Token::repr() 	const
{
	std::stringstream s;
		// TODO: make sure this matches enum Symbol exactly
	static std::string typeName[] = {
		"NONE", "FAIL", "END", "ID", "NUM", "STR", "BOOL",
		"O_PARAN", "C_PARAN", "O_BRACE", "C_BRACE", "O_BRACKET", "C_BRACKET",
		"PLUS_EQ", "PLUS", "MINUS", "TIMES", "DIVIDE",
		"LESS", "LESS_EQ", "EQ", "NOT_EQ", "GREATER", "GREATER_EQ",
		"IF", "ELIF", "ELSE", "WRITE", "PRINT", "DEF", "CLASS",
		"DO", "WHILE", "UNTIL", "FOR", "IN", "BREAK", "CONT", "READ",
		"ASSIGN", "INIT", "AND", "OR", "NOT", "COMMA", "SC", "QUIT"};
		
		s << '<' << typeName[getType()];

		if ((getType() == Sym::STR) || (getType() == Sym::FAIL))
			s << ", \"" << getStr() <<'"';
		else if (getType() == Sym::ID)
			s << ", " << getStr();
		else if (getType() == Sym::NUM)
			s << ", " << getNum();
		else if (getType() == Sym::NUM)
			s << ", " << std::boolalpha << getBool() << std::noboolalpha;
		s << '>';
		return s.str();
}

Token &Token::operator =(const Token &token)
{
	std::cerr << __FILE__ <<':'<<__LINE__<<token <<std::endl;
	if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
	{
		std::cerr << __FILE__ <<':'<<__LINE__<<std::endl;
		if (mVal.s)
			delete  mVal.s;
		mVal.s = NULL;
	}
	std::cerr << __FILE__ <<':'<<__LINE__<<token<<std::endl;
	mType = token.mType;
	
	if ((token.mType == Sym::ID) || (token.mType == Sym::FAIL) || (token.mType == Sym::STR))
	{
		std::cerr << __FILE__ <<':'<<__LINE__<<std::endl;
		mVal.s = new std::string(*token.mVal.s);
	}
	else if (token.mType == Sym::NUM)
	{
		std::cerr << __FILE__ <<':'<<__LINE__<<std::endl;
		mVal.d = token.mVal.d;
	}
	else if (token.mType == Sym::BOOL)
	{
		std::cerr << __FILE__ <<':'<<__LINE__<<std::endl;
		mVal.b = token.mVal.b;
	}
	
	return *this;
}

Token::operator Symbol() const
{
	return getType();
}

std::ostream &operator <<(std::ostream &stream, const Token &token)
{

	stream << token.repr();
	return stream;
}
