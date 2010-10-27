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
		sKeywords["do"]    	 =	Sym::DO;
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
		else if (ch == '+')
		{
			mInput->get();
			mToken = Sym::INCR;
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
		std::string mTokenStr;
		mTokenStr.reserve(10);
		Token t;
				
		if (ch == '-')
		{
			mInput->get();
			ch = mInput->peek();
			if (ch == '-')
			{
				t = Token(Sym::DECR);
				mInput->get();
			}
			else if ((!isdigit(ch) && (ch != '.')) 
				|| (mToken.getType() == Sym::NUM) || (mToken.getType() == Sym::ID)
				|| (mToken.getType() == Sym::STR))
				t = Token(Sym::MINUS);
			else
				mTokenStr += '-';
		}
		if (t.getType() != Sym::NONE)
			mToken = t;
		else
		{ 
			bool foundPoint = false;
			do
			{
				foundPoint = foundPoint || (ch == '.');
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



