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

namespace Sym {
   
   enum Symbol 
   {
      // Terminals:
      NONE = 0, FAIL, END, ID, NUM,  STR, T, F,
      O_PARAN, C_PARAN,O_BRACE, C_BRACE, O_BRACKET, C_BRACKET,
      PLUS_EQ, PLUS, MINUS, TIMES, DIVIDE,
      LESS, LESS_EQ, EQ, NOT_EQ, GREATER, GREATER_EQ,
      IF, ELIF, ELSE, PRINT, DEF, CLASS,
      DO, WHILE, UNTIL, FOR, IN, BREAK, CONT, READ,
      ASSIGN, INIT, AND, OR, NOT, COMMA, SC, EXIT,

#warning remove non-terminals if not being used      
//       // Non-terminals:
// 		CODE,
//       COND, COND2, COND_ELIF, COND_ELIF2, COND_ELSE, COND_ELSE2
// 		LOOP, WLOOP, DLOOP, FLOOP, IO_STMT
// 		BLOCK, STMT, ASGNMT, ASGNMT2,
// 		ORX, ORX2, ANDX, ANDX2, COMP, COMP2,
// 		EXPR, EXPR2, TERM, TERM2, FACTOR,
// 		OP0, OP1, OP2, OP3, OP4, OP5,
// 		LAST
      };
      
}

#endif // BS_SYMBOL_H
