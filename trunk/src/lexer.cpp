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

Token::Token(Symbol type) : mType(type), mVal(NULL) {}

Token::Token(Symbol type, const std::string &str) : mType(type), mVal(NULL)
{
   if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
		mVal = reinterpret_cast<void *>(new std::string(str));
	else
	{
      mType = Sym::FAIL;
		std::string FAILor = "FAILOR: only FAIL, STR, and ID types can have string values";
		mVal = reinterpret_cast<void *>(new std::string(FAILor));
	}
}

Token::Token(Symbol type, const double &num) : mType(type), mVal(NULL)
{
   if (mType == Sym::NUM)
		mVal = reinterpret_cast<void *>(new double(num));
	else
	{
      mType = Sym::FAIL;
		std::string FAILor = "FAILOR: only NUM type tokens can have numerical values";
		mVal = reinterpret_cast<void *>(new std::string(FAILor));
	}
}

Token::Token(const Token &token) : mType(token.mType), mVal(NULL)
{
	if (token.mVal)
	{
      if ((token.mType == Sym::ID) || (token.mType == Sym::FAIL) || (token.mType == Sym::STR))
			mVal = reinterpret_cast<void *>(new std::string(
						*reinterpret_cast<std::string *>(token.mVal)));
      else if (token.mType == Sym::NUM)
			mVal = reinterpret_cast<void *>(new double(
						*reinterpret_cast<double *>(token.mVal)));
	}

}

Token::~Token()
{
	if (mVal)
	{
      if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
			delete  reinterpret_cast<std::string *>(mVal);
      else if (mType == Sym::NUM)
			delete  reinterpret_cast<double *>(mVal);
		mVal = NULL;
	}
}

Symbol Token::getType() const { return mType; }

std::string Token::getStr() const
{
   if (mVal && ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR)))
		return *reinterpret_cast<std::string *>(mVal);
	else
		return "";
}

double Token::getNum() const
{
   if (mVal && mType == Sym::NUM)
		return *reinterpret_cast<double *>(mVal);
	else
		return 0.0;
}

Token &Token::operator =(const Token &token)
{
	if (mVal)
	{
      if ((mType == Sym::ID) || (mType == Sym::FAIL) || (mType == Sym::STR))
			delete  reinterpret_cast<std::string *>(mVal);
      else if (mType == Sym::NUM)
			delete  reinterpret_cast<double *>(mVal);
		mVal = NULL;
	}

	mType = token.mType;
	
	if (token.mVal)
	{
      if ((token.mType == Sym::ID) || (token.mType == Sym::FAIL) || (token.mType == Sym::STR))
			mVal = reinterpret_cast<void *>(new std::string(
						*reinterpret_cast<std::string *>(token.mVal)));
      else if (token.mType == Sym::NUM)
			mVal = reinterpret_cast<void *>(new double(
						*reinterpret_cast<double *>(token.mVal)));
	}
}


std::ostream &operator <<(std::ostream &stream, const Token &token)
{
	// TODO: make sure this matches enum Symbol exactly
   static std::string typeName[] = {"NONE", "FAIL", "END", "ID", "NUM", "STR", "T", "F",
		"O_PARAN", "C_PARAN", "O_BRACE", "C_BRACE", "O_BRACKET", "C_BRACKET",
      "PLUS_EQ", "PLUS", "MINUS", "TIMES", "DIVIDE",
      "LESS", "LESS_EQ", "EQ", "NOT_EQ", "GREATER", "GREATER_EQ",
      "IF", "ELIF", "ELSE", "PRINT", "DEF", "CLASS",
      "DO", "WHILE", "UNTIL", "FOR", "IN", "BREAK", "CONT", "READ",
      "ASSIGN", "INIT", "AND", "OR", "NOT", "COMMA", "SC"};
		
	stream << '<' << typeName[token.getType()];

	if ((token.getType() == Sym::STR) || (token.getType() == Sym::FAIL))
		stream << ", \"" << token.getStr() <<'"';
	else if (token.getType() == Sym::ID) 
		stream << ", " << token.getStr();
	else if (token.getType() == Sym::NUM)
		stream << ", " << token.getNum();
	stream << '>';
	return stream;
}


Lexer::Lexer(std::istream &input)
   : mInput(&input), mLineNum(0), mToken(Sym::END) 
{
	 setupKeywords(); 
}

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
		sKeywords["print"] 	 =	Sym::PRINT;
		sKeywords["def"] 		 =	Sym::DEF;
		sKeywords["class"] 	 = Sym::CLASS;
		
      sKeywords["and"]		 = Sym::AND;
		sKeywords["or"]		 =	Sym::OR;
		sKeywords["not"]		 =	Sym::NOT;
      
      sKeywords["true"]     = Sym::T;
      sKeywords["false"]    = Sym::F;
	}
}

const Token &Lexer::getNext()
{
	
	char ch = mInput->peek();
	
// TODO: reenable read file FAILure?
// 	if (mInput->FAIL())
// 		mToken = Token(FAIL, "reading file FAILed");
	/*else*/ if (mInput->eof())
					mToken = Token(Sym::END);
	else if (ch == '#')
	{
		do
		{
			mInput->get();
			ch = mInput->peek();
		} while (!(mInput->eof()) && (ch != '\n'));
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
		
		std::map <std::string, Sym::Symbol>::iterator it = sKeywords.find(mTokenStr);
		if (it != sKeywords.end()) // keyword
			mToken = Token(it->second);
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
		// TODO: make "4-2" return <NUM, 4> <MINUS> <NUM, 2> NOT <NUM, 4> <NUM, -2>
		bool minus    = false;
		std::string mTokenStr;
		mTokenStr.reserve(10);
				
		if (ch == '-')
		{
			mInput->get();
			ch = mInput->peek();
			if ((!isdigit(ch) && (ch != '.')) 
				|| (mToken.getType() == Sym::NUM) || (mToken.getType() == Sym::ID)) 
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
			mToken = Token(Sym::EQ);
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



