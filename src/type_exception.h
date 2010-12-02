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
#ifndef BS_TYPE_EXCEPTION_H
#define BS_TYPE_EXCEPTION_H
#include <exception>
class TypeException : public std::exception
{
	public:
		TypeException(const char* description = "") : mWhat(description) {}
		~TypeException() throw() { delete [] mWhat; }
		const char *what() throw() { return mWhat; }
	private:
		const char *mWhat;
};

#endif // ndef BS_TYPE_EXCEPT_H
