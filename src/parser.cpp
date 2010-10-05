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

#include "parser.h"

Parser::Parser(std::istream &input) : mLexer(input) {}

void Parser::setupTable()
{
	if (sTable.empty())
	{
		sTable[TERM][O_PARAN]  = 100; // TERM -> FACTOR TERM2
		sTable[TERM][F]        = 100; 
		sTable[TERM][T]        = 100;
		sTable[TERM][ID]       = 100;
		sTable[TERM][NUM]      = 100;
		sTable[TERM][STR]      = 100;
		
		sTable[TERM2][C_PARAN] = 110; // TERM2 -> e
		sTable[TERM2][PLUS]    = 111; // TERM2 -> PLUS  TERM
		sTable[TERM2][MINUS]   = 112; // TERM2 -> MINUS TERM
		
		sTable[FACTOR][O_PARAN]= 120; // FACTOR -> ( TERM )
		sTable[FACTOR][STR]    = 121; // FACTOR -> STR
		sTable[FACTOR][ID]     = 122; // FACTOR -> ID
		sTable[FACTOR][NUM]    = 123; // FACTOR -> NUM
		sTable[FACTOR][T]      = 124; // FACTOR -> T
		sTable[FACTOR][F]      = 125; // FACTOR -> F
	}
}

void Parser::act(int action)
{
	
}
