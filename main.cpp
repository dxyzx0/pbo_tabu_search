//
// Created by psw on 5/19/24.
//

#include <iostream>
#include <cassert>
#include "parser/SimpleParser.h"
#include "parser/AbcCallback.h"
#include "Problem.h"
#include "Solver.h"

using namespace std;


int main(int argc, char* argv[])
{
	SimpleParser< AbcCallback >* parser = nullptr;
	try
	{
		if (argc != 2)
		{
			cout << "usage: pbo_heur_eigen <filename>" << endl;
			return -1;
		}
		else
		{
			string filename = argv[1];
			// assert filename ends with ".opb" or ".pb"
			cout << "Parsing file: " << filename << endl;
			assert(filename.substr(filename.find_last_of('.') + 1) == "opb" || filename.substr(filename.find_last_of('.') + 1) == "pb");
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

	Solver solver(make_shared< Problem >(parser->cb), make_shared< Settings >());
	SolveResult result = solver.solve();

	cout << "Result: " << result << endl;

	return 0;
}