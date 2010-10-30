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
      NONE = 0, FAIL, END, ID, NUM,  STR, BOOL, 
      O_PARAN, C_PARAN,O_BRACE, C_BRACE, O_BRACKET, C_BRACKET,
      PLUS_EQ, PLUS, MINUS, TIMES, DIVIDE,
      LESS, LESS_EQ, EQ, NOT_EQ, GREATER, GREATER_EQ,
      IF, ELIF, ELSE, WRITE, PRINT, DEF, CLASS,
      DO, WHILE, UNTIL, FOR, IN, BREAK, CONT, READ,
      ASSIGN, INIT, AND, OR, NOT, INCR, DECR, TYPE, COMMA, SC, QUIT

      };
      
}

#endif // BS_SYMBOL_H
