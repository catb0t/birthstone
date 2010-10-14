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

#include "lexer.h"
#include <ostream>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

std::map <std::string, Token> Lexer::sKeywords;

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
	if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
	{
		if (mVal.s)
			delete  mVal.s;
		mVal.s = NULL;
	}

	mType = token.mType;
	
      if ((token.mType == Sym::ID) || (token.mType == Sym::FAIL) || (token.mType == Sym::STR))
			mVal.s = new std::string(*token.mVal.s);
      else if (token.mType == Sym::NUM)
			mVal.d = token.mVal.d;
		else if (token.mType == Sym::BOOL)
			mVal.b = token.mVal.b;
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


Lexer::Lexer(std::istream &input)
   : mInput(&input), mLineNum(1), mToken(Sym::END)
{
	 setupKeywords(); 
}

Lexer::Lexer(const Lexer &lexer)
	: mInput(lexer.mInput), mLineNum(1), mToken(Sym::END)
{}

Lexer::~Lexer() {}

void Lexer::setupKeywords()
{
	if (sKeywords.empty())
	{
		sKeywords["if"]       =	Sym::IF;
		sKeywords["elif"]     = Sym::ELIF;
		sKeywords["else"]     =	Sym::ELSE;
		sKeywords["while"]    =	Sym::WHILE;
		sKeywords["until"]    =	Sym::UNTIL;
		sKeywords["for"]      =	Sym::FOR;
		sKeywords["in"]       =	Sym::IN;
		sKeywords["break"]    =	Sym::BREAK;
		sKeywords["continue"] =	Sym::CONT;
		sKeywords["read"] 	 =	Sym::READ;
		sKeywords["write"] 	 =	Sym::WRITE;
		sKeywords["print"] 	 =	Sym::PRINT;
		sKeywords["def"] 		 =	Sym::DEF;
		sKeywords["class"] 	 = Sym::CLASS;
		
      sKeywords["and"]		 = Sym::AND;
		sKeywords["or"]		 =	Sym::OR;
		sKeywords["not"]		 =	Sym::NOT;
      
		sKeywords["true"]     = Token(Sym::BOOL, true);
		sKeywords["false"]    = Token(Sym::BOOL, false);

		sKeywords["exit"]     = Sym::QUIT;
		sKeywords["quit"]     = Sym::QUIT;
	}
}

const Token &Lexer::getNext()
{
	
	char ch = mInput->peek();
	
	if (mInput->eof())
		mToken = Token(Sym::END);
	else if (ch == '#')
	{
		do
		{
			mInput->get();
			ch = mInput->peek();
		} while (!(mInput->eof()) && (ch != '\n'));
		++mLineNum;
		mInput->get();
		return getNext();
	}
	else if (ch == ' ' || ch == '\t')
	{
		mInput->get();
		return getNext();
	}
	else if (ch == '\n')
	{
		++mLineNum;
		mInput->get();
		return getNext();
	}

	else if (ch == '(')
	{
		mInput->get();
		mToken = Sym::O_PARAN;
	}
	else if (ch == ')')
	{
		mInput->get();
		mToken = Sym::C_PARAN;
	}
	else if (ch == '[')
	{
		mInput->get();
		mToken = Sym::O_BRACKET;
	}
	else if (ch == ']')
	{
		mInput->get();
		mToken = Sym::C_BRACKET;
	}
	else if (ch == '{')
	{
		mInput->get();
		mToken = Sym::O_BRACE;
	}
	else if (ch == '}')
	{
		mInput->get();
		mToken = Sym::C_BRACE;
	}
	else if (ch == ';')
	{
		mInput->get();
		mToken = Sym::SC;
	}
	else if (ch == ',')
	{
		mInput->get();
		mToken = Sym::COMMA;
	}
	else if (isalpha(ch) || ch == '_')
	{
		std::string mTokenStr;
		mTokenStr.reserve(20);
		do
		{
			mTokenStr += ch;
			mInput->get();
			ch = mInput->peek();
		} while (!(mInput->eof()) && (isalnum(ch) || ch == '_'));
		
		std::map <std::string, Token>::iterator it = sKeywords.find(mTokenStr);
		if (it != sKeywords.end()) // keyword
			mToken = it->second;
		else //identifier
			mToken = Token(Sym::ID, mTokenStr);
	}
   else if ((ch == '"') || (ch == '\''))
	{
      char quote = ch;
		std::string mTokenStr;
		mTokenStr.reserve(20);

		mInput->get();
		ch = mInput->peek();

      while (!(mInput->eof()) && (ch != quote))
		{
			if (ch == '\\')
			{
				mInput->get();
				ch = mInput->peek();
				switch (ch)
				{
					case 't':
						ch = '\t';
						break;
					case 'n':
						ch = '\n';
						break;
					// \\, \', \" will work because we just want to add the
					//     character after the \ to the string
				}
			}
			mTokenStr += ch;
			mInput->get();
			ch = mInput->peek();
         
		} 
		mInput->get();
		mToken = Token(Sym::STR, mTokenStr);
	}
	else if (ch == '+')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Sym::PLUS_EQ;
		}
		else
			mToken = Sym::PLUS;
	}
	else if (ch == '*')
	{
		mInput->get();
		mToken = Sym::TIMES;
	}
	else if (ch == '/')
	{
		mInput->get();
		mToken = Sym::DIVIDE;
	}
	else if ((ch == '-') || (ch == '.') || isdigit(ch))
	{
		bool minus    = false;
		std::string mTokenStr;
		mTokenStr.reserve(10);
				
		if (ch == '-')
		{
			mInput->get();
			ch = mInput->peek();
			if ((!isdigit(ch) && (ch != '.')) 
				|| (mToken.getType() == Sym::NUM) || (mToken.getType() == Sym::ID)
				|| (mToken.getType() == Sym::STR))
				minus = true;
			else
				mTokenStr += '-';
		}
		if (minus)
			mToken = Token(Sym::MINUS);
		else
		{
			bool foundPoint = false;
			do
			{
				foundPoint = (ch == '.') || foundPoint;
				mTokenStr += ch;
				mInput->get();
				ch = mInput->peek();
			} while(!(mInput->eof()) && (isdigit(ch) || (!foundPoint && (ch == '.'))));
			mToken = Token(Sym::NUM, atof(mTokenStr.c_str()));
		}
	}
	else if (ch == '<')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Sym::LESS_EQ);
		}
		else
			mToken = Token(Sym::LESS);
	}
	else if (ch == '>')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Sym::GREATER_EQ);
		}
		else
			mToken = Token(Sym::GREATER);
	}
	else if (ch == '=')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Sym::EQ);
		}
		else
			mToken = Token(Sym::ASSIGN);
	}
	else if (ch == '&')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '&')
		{
			mInput->get();
			mToken = Token(Sym::AND);
		}
		else
			mToken = Token(Sym::FAIL, std::string("expected: \"&&\" found \"&") + ch + "\"");
	}
	
	else if (ch == '|')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '|')
		{
			mInput->get();
			mToken = Token(Sym::OR);
		}
		else
			mToken = Token(Sym::FAIL, std::string("expected: \"||\" found \"|") + ch + "\"");
	}
	else if (ch == '!')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Sym::NOT_EQ);
		}
		else
			mToken = Token(Sym::NOT);
	}
	else if (ch == ':')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Sym::INIT);
		}
		else
			mToken = Token(Sym::FAIL, std::string("expected: ':=' found ':") + ch + "'");
	}
	else
	{
		mInput->get();
		mToken = Token(Sym::FAIL, std::string("Invalid mToken: '") + ch + '\'');
	}
	
	return mToken;
}

unsigned int Lexer::getLine() const { return mLineNum; }

Lexer &Lexer::operator=(const Lexer &lexer)
{
		mInput = lexer.mInput;
}

#ifdef BS_LEXER_DEBUG

#include <iostream>
#include <sstream>
	int main ()
	{

		while (true)
		{
			std::string str;
			std::stringstream stream;
			std::cout << "Type some commands (Ctrl-C to exit): ";
			std::getline(std::cin, str);
			stream.str(str);
			Lexer lex(stream);
			Token token = lex.getNext();
			
			std::cout << "   ";
			while ((token.getType() != Sym::END) /*&& (token.getType() != FAIL)*/)
			{
 				std::cout << token << ' ';
				token = lex.getNext();
			}
			// 			std::cFAIL << token; // show the <END> token
			std::cout << std::endl;
		}
	}

#endif // BS_LEXER_DEBUG



