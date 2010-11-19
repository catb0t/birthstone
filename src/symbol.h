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
#ifndef BS_SYMBOL_H
#define BS_SYMBOL_H
#include <map>
#include <string>

enum Symbol
{
	S_NONE = 0, S_FAIL, S_END, S_ID, S_NUM,  S_STR, S_BOOL, S_LIST, S_FUNC, S_FILE,
	S_O_PARAN, S_C_PARAN, S_O_BRACE, S_C_BRACE, S_O_BRACKET, S_C_BRACKET,
	S_PLUS_EQ, S_PLUS, S_MINUS, S_TIMES, S_DIVIDE,
	S_LESS, S_LESS_EQ, S_EQ, S_NOT_EQ, S_GREATER, S_GREATER_EQ,
	S_IF, S_ELIF, S_ELSE,
	S_OPEN, S_CLOSE, S_FREAD, S_FWRITE, S_FPRINT,
	S_WRITE, S_PRINT,	S_READ, S_DEF, S_CLASS, S_RET, S_DEL,
	S_DO, S_WHILE, S_UNTIL, S_FOR, S_IN, S_BREAK, S_CONT,  
	S_ASSIGN, S_INIT, S_AND, S_OR, S_NOT, S_INCR, S_DECR, S_TYPE, S_COMMA, S_SC, S_QUIT

};

const std::string symName[] = {
	"NONE ", "FAIL", "END", "ID", "NUM", " STR", "BOOL", "LIST", "FUNC", "FILE",
	"O_PARAN", "C_PARAN", "O_BRACE", "C_BRACE", "O_BRACKET", "C_BRACKET",
	"PLUEQ", "PLUS", "MINUS", "TIMES", "DIVIDE",
	"LESS", "LESEQ", "EQ", "NOT_EQ", "GREATER", "GREATER_EQ",
	"IF", "ELIF", "ELSE",
	"OPEN", "CLOSE", "FREAD", "FWRITE", "FPRINT",
	"WRITE", "PRINT", "READ", "DEF", "CLASS", "RET", "DEL",
	"DO", "WHILE", "UNTIL", "FOR", "IN", "BREAK", "CONT",
	"ASSIGN", "INIT", "AND", "OR", "NOT", "INCR", "DECR", "TYPE", "COMMA", "SC", "QUIT"
		
};
      

#endif // BS_SYMBOL_H
