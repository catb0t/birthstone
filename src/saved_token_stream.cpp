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
#include "token_stream.h"

#ifdef BS_DEBUG
#include <iostream>
#endif // BS_DEBUG

SavedTokenStream::SavedTokenStream() : mStream(), mPos(mStream.begin()), mToken(Sym::END)
{
	mStream.reserve(20);
}

const Token &SavedTokenStream::getNext()
{
	if (mStream.empty() || (mPos == mStream.end()))
	{
		mToken = Token(Sym::END);
	}
	else
	{
		mToken = *mPos;
		++mPos;
	}
	return mToken;
}


void SavedTokenStream::addToken(Token token)
{
	mStream.push_back(token);
	rewind();

	#ifdef BS_DEBUG
	std::cerr << "token stream: ";
	for ( std::vector<Token>::iterator i = mStream.begin(); i != mStream.end(); ++i)
		std::cerr << *i << " ";
	std::cerr << std::endl;

	#endif // BS_DEBUG		
}

void SavedTokenStream::rewind()
{
	mPos = mStream.begin();
}
