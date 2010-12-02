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

SavedTokenStream::SavedTokenStream() : mStream(), mToken(S_END) {}

const Token &SavedTokenStream::getNext()
{
	if (mStream.empty())
		mToken = Token(S_END);
	else
	{
		mToken = mStream.front();
		mStream.push(mToken);
		mStream.pop();
	}
	return mToken;
}

bool SavedTokenStream::empty() const {	return mStream.empty(); }

void SavedTokenStream::add(const Token &token) { mStream.push(token); }
