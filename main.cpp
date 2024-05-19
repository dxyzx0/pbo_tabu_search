//
// Created by psw on 5/19/24.
//

#include <iostream>
#include "parser/SimpleParser.h"
#include "parser/AbcCallback.h"
#include "Problem.h"

using namespace std;


int main(int argc, char* argv[])
{
	SimpleParser< AbcCallback >* parser = nullptr;
	try
	{
		if (argc != 2)
		{
			cout << "usage: SimpleParser <filename>" << endl;
			return -1;
		}
		else
		{
			parser = new SimpleParser< AbcCallback >(argv[1]);

			parser->setAutoLinearize(true);
			parser->parse();
		}
	}
	catch (exception& e)
	{
		cout.flush();
		cerr << "ERROR: " << e.what() << endl;
	}

	assert(parser != nullptr);
	Problem problem(parser->cb);

	return 0;
}