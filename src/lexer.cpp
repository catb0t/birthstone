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
#include <iostream>

Token::Token(TokenType type) : mType(type), mVal(NULL) {}

Token::Token(TokenType type, const std::string &str) : mType(type), mVal(NULL)
{
	if ((mType == ID) || (mType == FAIL) || (mType == STR))
		mVal = reinterpret_cast<void *>(new std::string(str));
	else
	{
		mType = FAIL;
		std::string error = "ERROR: only FAIL, STR, and ID types can have string values";
		mVal = reinterpret_cast<void *>(new std::string(error));
	}
}

Token::Token(TokenType type, const double &num) : mType(type), mVal(NULL)
{
	if (mType == NUM)
		mVal = reinterpret_cast<void *>(new double(num));
	else
	{
		mType = FAIL;
		std::string error = "ERROR: only NUM type tokens can have numerical values";
		mVal = reinterpret_cast<void *>(new std::string(error));
	}
}

Token::Token(const Token &token) : mType(token.mType), mVal(NULL)
{
	if (token.mVal)
	{
		if ((token.mType == ID) || (token.mType == FAIL) || (token.mType == STR))
			mVal = reinterpret_cast<void *>(new std::string(
						*reinterpret_cast<std::string *>(token.mVal)));
		else if (token.mType == NUM)
			mVal = reinterpret_cast<void *>(new double(
						*reinterpret_cast<double *>(token.mVal)));
	}

}

Token::~Token()
{
	if (mVal)
	{
		if ((mType == ID) || (mType == FAIL) || (mType == STR))
			delete  reinterpret_cast<std::string *>(mVal);
		else if (mType == NUM)
			delete  reinterpret_cast<double *>(mVal);
		mVal = NULL;
	}
}

Token::TokenType Token::getType() const { return mType; }

std::string Token::getStr() const
{
	if (mVal && ((mType == ID) || (mType == FAIL) || (mType == STR)))
		return *reinterpret_cast<std::string *>(mVal);
	else
		return "";
}

double Token::getNum() const
{
	if (mVal && mType == NUM)
		return *reinterpret_cast<double *>(mVal);
	else
		return 0.0;
}

Token &Token::operator =(const Token &token)
{
	if (mVal)
	{
		if ((mType == ID) || (mType == FAIL) || (mType == STR))
			delete  reinterpret_cast<std::string *>(mVal);
		else if (mType == NUM)
			delete  reinterpret_cast<double *>(mVal);
		mVal = NULL;
	}

	mType = token.mType;
	
	if (token.mVal)
	{
		if ((token.mType == ID) || (token.mType == FAIL) || (token.mType == STR))
			mVal = reinterpret_cast<void *>(new std::string(
						*reinterpret_cast<std::string *>(token.mVal)));
		else if (token.mType == NUM)
			mVal = reinterpret_cast<void *>(new double(
						*reinterpret_cast<double *>(token.mVal)));
	}
}


std::ostream &operator <<(std::ostream &stream, const Token &token)
{
	// TODO: make sure this matches enum TokenType exactly
	static std::string typeName[] = {"NONE", "FAIL", "END", "ID", "NUM", "STR",
		"O_PARAN", "C_PARAN", "O_BRACE", "C_BRACE", "O_BRACKET", "C_BRACKET",
      "PLUS_EQ", "PLUS", "MINUS", "TIMES", "DIVIDE",
      "LESS", "LESS_EQ", "EQ", "NOT_EQ", "GREATER", "GREATER_EQ",
      "IF", "ELIF", "ELSE", "PRINT", "DEF", "CLASS",
      "DO", "WHILE", "UNTIL", "FOR", "IN", "BREAK", "CONT", "READ",
      "ASSIGN", "INIT", "AND", "OR", "NOT", "COMMA", "SC"};
		
	stream << '<' << typeName[token.getType()];

	if ((token.getType() == Token::STR) || (token.getType() == Token::FAIL))
		stream << ", \"" << token.getStr() <<'"';
	else if (token.getType() == Token::ID) 
		stream << ", " << token.getStr();
	else if (token.getType() == Token::NUM)
		stream << ", " << token.getNum();
	stream << '>';
	return stream;
}


Lexer::Lexer(std::istream *input) 
	: mInput(input), mLineNum(0), mToken(Token::END)  
{ 
	setupKeywords(); 
}

Lexer::Lexer(std::istream &input) 
	: mInput(&input), mLineNum(0), mToken(Token::END) 
{
	 setupKeywords(); 
}

Lexer::~Lexer() {}

void Lexer::setupKeywords()
{
	if (sKeywords.empty())
	{
		sKeywords["if"]       =	Token::IF;
		sKeywords["elif"]     = Token::ELIF;
		sKeywords["else"]     =	Token::ELSE;
		sKeywords["while"]    =	Token::WHILE;
		sKeywords["until"]    =	Token::UNTIL;
		sKeywords["for"]      =	Token::FOR;
		sKeywords["in"]       =	Token::IN;
		sKeywords["break"]    =	Token::BREAK;
		sKeywords["continue"] =	Token::CONT;
		sKeywords["read"] 	 =	Token::READ;
		sKeywords["print"] 	 =	Token::PRINT;
		sKeywords["def"] 		 =	Token::DEF;
		sKeywords["class"] 	 = Token::CLASS;
		sKeywords["and"]		 = Token::AND;
		sKeywords["or"]		 =	Token::OR;
		sKeywords["not"]		 =	Token::NOT;
	}
}

const Token &Lexer::getNext()
{
	
	char ch = mInput->peek();
	
// TODO: reenable read file failure?
// 	if (mInput->fail())
// 		mToken = Token(FAIL, "reading file failed");
	/*else*/ if (mInput->eof())
					mToken = Token(Token::END);
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
		mToken = Token::O_PARAN;
	}
	else if (ch == ')')
	{
		mInput->get();
		mToken = Token::C_PARAN;
	}
	else if (ch == '[')
	{
		mInput->get();
		mToken = Token::O_BRACKET;
	}
	else if (ch == ']')
	{
		mInput->get();
		mToken = Token::C_BRACKET;
	}
	else if (ch == '{')
	{
		mInput->get();
		mToken = Token::O_BRACE;
	}
	else if (ch == '}')
	{
		mInput->get();
		mToken = Token::C_BRACE;
	}
	else if (ch == ';')
	{
		mInput->get();
		mToken = Token::SC;
	}
	else if (ch == ',')
	{
		mInput->get();
		mToken = Token::COMMA;
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
		
		std::map <std::string, Token::TokenType>::iterator it = sKeywords.find(mTokenStr);
		if (it != sKeywords.end()) // keyword
			mToken = Token(it->second);
		else //identifier
			mToken = Token(Token::ID, mTokenStr);
	}
	else if (ch == '"')
	{
		std::string mTokenStr;
		mTokenStr.reserve(20);

		mInput->get();
		ch = mInput->peek();

		while (ch != '"')
		{
			mTokenStr += ch;
			mInput->get();
			ch = mInput->peek();
		} 
		mInput->get();
		mToken = Token(Token::STR, mTokenStr);
	}
	else if (ch == '+')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token::PLUS_EQ;
		}
		else
			mToken = Token::PLUS;
	}
	else if (ch == '*')
	{
		mInput->get();
		mToken = Token::TIMES;
	}
	else if (ch == '/')
	{
		mInput->get();
		mToken = Token::DIVIDE;
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
				|| (mToken.getType() == Token::NUM) || (mToken.getType() == Token::ID)) 
				minus = true;
			else
				mTokenStr += '-';
		}
		if (minus)
			mToken = Token(Token::MINUS);
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
			mToken = Token(Token::NUM, atof(mTokenStr.c_str()));
		}
	}
	else if (ch == '<')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Token::LESS_EQ);
		}
		else
			mToken = Token(Token::LESS);
	}
	else if (ch == '>')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Token::GREATER_EQ);
		}
		else
			mToken = Token(Token::GREATER);
	}
	else if (ch == '=')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Token::EQ);
		}
		else
			mToken = Token(Token::ASSIGN);
	}
	else if (ch == '&')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '&')
		{
			mInput->get();
			mToken = Token(Token::AND);
		}
		else
			mToken = Token(Token::FAIL, std::string("expected: \"&&\" found \"&") + ch + "\"");
	}
	
	else if (ch == '|')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '|')
		{
			mInput->get();
			mToken = Token(Token::OR);
		}
		else
			mToken = Token(Token::FAIL, std::string("expected: \"||\" found \"|") + ch + "\"");
	}
	else if (ch == '!')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Token::EQ);
		}
		else
			mToken = Token(Token::NOT);
	}
	else if (ch == ':')
	{
		mInput->get();
		ch = mInput->peek();
		if (ch == '=')
		{
			mInput->get();
			mToken = Token(Token::INIT);
		}
		else
			mToken = Token(Token::FAIL, std::string("expected: ':=' found ':") + ch + "'");
	}
	else
	{
		mInput->get();
		mToken = Token(Token::FAIL, std::string("Invalid mToken: '") + ch + '\'');
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
			
			std::cout << '\t';
			while ((token.getType() != Token::END) /*&& (token.getType() != FAIL)*/)
			{
 				std::cout << token << ' ';
				token = lex.getNext();
			}
			// 			std::cerr << token; // show the <END> token
			std::cout << std::endl;
		}
	}

#endif // BS_LEXER_DEBUG



