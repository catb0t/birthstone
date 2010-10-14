/******************************************************************************
*	  Birthstone is a programming language and interpreter language written    *
*   by Robb Tolliver for his Senior Project at BYU-Idaho during the Fall      *
*   Semester of 2010.                                                         *
*                                                                             *
*   Copyright (C) 2010 by Robert Tolliver                                     *
*   Robb.Tolli@gmail.com                                                      *
*                                                                             *
*   Birthstone is free software: you can redistribute it and/or modify        *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation, either version 3 of the License, or         *
*   (at your option) any later version.                                       *
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
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "parser.h"
using namespace std;


int main(int argc, char **argv)
{
	string filename;
	bool interactive = true;
	if (argc > 1)
	{
		ifstream input(argv[1]);
		if (input.good())
			Parser(input).run();
		else
		{
			cerr << "error: could not read input file. " << endl;
		}
		input.close();
	}
	else // interactive mode
	{
		string str;
		stringstream input;
		Parser parser(input);
		cout << "birthstone interactive shell" << endl;
		do
		{

			cout << "bs> ";
			getline(cin, str);
			input.seekg(0, ios_base::beg);
			input.str(str);

			parser.newInput(input);
			parser.run();
		} while (true/* TODO:  stop when quit or exit is entered*/);
	}

	
	return 0;
}
