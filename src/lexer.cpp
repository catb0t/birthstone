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


Lexer::Lexer(std::istream &input)
   : mInput(&input), mLineNum(1), mToken(S_END)
{}

Lexer::Lexer(const Lexer &lexer)
	: mInput(lexer.mInput), mLineNum(1), mToken(S_END)
{}

Lexer::~Lexer() {}

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
const std::map <std::string, Token> &Lexer::keywords()
{
	using namespace boost::assign; // bring 'map_list_of()' into scope
	// http://www.boost.org/doc/libs/1_45_0/libs/assign/doc/index.html#map_list_of
	
	static std::map <std::string, Token> keywords
		= map_list_of ("None", noTkn) ("if", S_IF) ("elif", S_ELIF)
   	("elsif", S_ELIF)("elseif", S_ELIF)("else", S_ELSE)("do", S_DO)
		("while", S_WHILE)("until", S_UNTIL)("for", S_FOR)("in", S_IN)
		("break", S_BREAK)("continue", S_CONT)("read", S_READ)("write", S_WRITE)
		("print", S_PRINT)("open", S_OPEN)("close", S_CLOSE) ("fread", S_FREAD)
		("fwrite", S_FWRITE)("fprint", S_FPRINT)("delete", S_DEL)("def", S_DEF)
		("class", S_CLASS)("return", S_RET)("and", S_AND)("or", S_OR)
		("not", S_NOT)("type", S_TYPE)("true", trueTkn)("false", falseTkn)
		("exit", S_QUIT)("quit", S_QUIT);

		return keywords;
	
}


const Token &Lexer::getNext()
{
	
	char ch = mInput->peek();
	
	if (mInput->eof())
		mToken = Token(S_END);
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
		mToken = S_O_PARAN;
	}
	else if (ch == ')')
	{
		mInput->get();
		mToken = S_C_PARAN;
	}
	else if (ch == '[')
	{
		mInput->get();
		mToken = S_O_BRACKET;
	}
	else if (ch == ']')
	{
		mInput->get();
		mToken = S_C_BRACKET;
	}
	else if (ch == '{')
	{
		mInput->get();
		mToken = S_O_BRACE;
	}
	else if (ch == '}')
	{
		mInput->get();
		mToken = S_C_BRACE;
	}
	else if (ch == ';')
	{
		mInput->get();
		mToken = S_SC;
	}
	else if (ch == ',')
	{
		mInput->get();
		mToken = S_COMMA;
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
		
		std::map <std::string, Token>::const_iterator it = keywords().find(mTokenStr);
		if (keywords().find(mTokenStr) != keywords().end()) // keyword
			mToken = it->second;
		else //identifier
			mToken = Token(S_ID, mTokenStr);
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
		mToken = Token(S_STR, mTokenStr);
	}
	else if (ch == '+')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = S_PLUS_EQ;
		}
		else if (ch == '+')
		{
			mInput->get();
			mToken = S_INCR;
		}
		else
			mToken = S_PLUS;
	}
	else if (ch == '*')
	{
		mInput->get();
		mToken = S_TIMES;
	}
	else if (ch == '/')
	{
		mInput->get();
		mToken = S_DIVIDE;
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
				t = Token(S_DECR);
				mInput->get();
			}
			else if ((!isdigit(ch) && (ch != '.')) 
				|| (mToken.getType() == S_NUM) || (mToken.getType() == S_ID)
				|| (mToken.getType() == S_STR))
				t = Token(S_MINUS);
			else
				mTokenStr += '-';
		}
		if (t.getType() != S_NONE)
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
			mToken = Token(atof(mTokenStr.c_str()));
		}

	}
	else if (ch == '<')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(S_LESS_EQ);
		}
		else
			mToken = Token(S_LESS);
	}
	else if (ch == '>')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(S_GREATER_EQ);
		}
		else
			mToken = Token(S_GREATER);
	}
	else if (ch == '=')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(S_EQ);
		}
		else
			mToken = Token(S_ASSIGN);
	}
	else if (ch == '&')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '&')
		{
			mInput->get();
			mToken = Token(S_AND);
		}
		else
			mToken = Token(S_FAIL, std::string("expected: \"&&\" found \"&") + ch + "\"");
	}
	
	else if (ch == '|')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '|')
		{
			mInput->get();
			mToken = Token(S_OR);
		}
		else
			mToken = Token(S_FAIL, std::string("expected: \"||\" found \"|") + ch + "\"");
	}
	else if (ch == '!')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(S_NOT_EQ);
		}
		else
			mToken = Token(S_NOT);
	}
	else if (ch == ':')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(S_INIT);
		}
		else
			mToken = Token(S_FAIL, std::string("expected: ':=' found ':") + ch + "'");
	}
	else
	{
		mInput->get();
		mToken = Token(S_FAIL, std::string("Invalid token: '") + ch + '\'');
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
			while ((token.getType() != S_END) /*&& (token.getType() != FAIL)*/)
			{
 				std::cout << token << ' ';
				token = lex.getNext();
			}
			// 			std::cFAIL << token; // show the <END> token
			std::cout << std::endl;
		}
	}

#endif // BS_LEXER_DEBUG



