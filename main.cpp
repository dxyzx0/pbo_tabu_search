//
// Created by psw on 5/19/24.
//

#include <iostream>
#include <cassert>
#include <chrono>
#include <spdlog/spdlog.h>
#include "parser/SimpleParser.h"
#include "parser/PboCallback.h"
#include "Problem.h"
#include "Solver.h"
#include "utils.h"

using namespace std;

int printUsage()
{
	printf("Usage: pbo_heur_eigen [--jobs|-j JOBS] [--timeout|-t TIMEOUT] [--verbose|-v] INFILE\n");
	return 1;
}

int main(int argc, char* argv[])
{
	spdlog::set_pattern(fmt::format("{}[%H:%M:%S %z] [%^%l%$] [thread %t] %v", PBO_LOG_COMMENT_PREFIX));

	shared_ptr< Settings > set = make_shared< Settings >();

	set->numthreads = 1;
	set->timelimit = 1e20;
	set->verbose = 0;
	set->startTime = std::chrono::steady_clock::now();

	std::string inputPath;
	for (int i = 1; i < argc; i++)
	{
		std::string argvi(argv[i]);
		if (argvi == "--timeout" || argvi == "-t")
		{
			if (i + 1 < argc)
				set->timelimit = std::stod(argv[i + 1]);
			else
				return printUsage();
			i++;
		}
		else if (argvi == "--verbose" || argvi == "-v")
		{
			set->verbose++;
			spdlog::set_level(spdlog::level::debug);
		}
		else if (argvi == "--jobs" || argvi == "-j")
		{
			if (i + 1 < argc)
				set->numthreads = std::stoi(argv[i + 1]);
			else
				return printUsage();
			i++;
		}
		else if (!inputPath.empty())
			return printUsage();
		else
			inputPath = argvi;
	}

	SimpleParser< PboCallback >* parser = nullptr;
	try
	{
		const string& filename = inputPath;
		// assert filename ends with ".opb" or ".pb"
		cout_com << "Parsing file: " << filename << endl;
		assert(filename.substr(filename.find_last_of('.') + 1) == "opb"
			|| filename.substr(filename.find_last_of('.') + 1) == "pb");
		parser = new SimpleParser< PboCallback >(filename.c_str());

		parser->setAutoLinearize(true);
		parser->parse();
	}
	catch (exception& e)
	{
		cout.flush();
		cerr << "ERROR: " << e.what() << endl;
		return -1;
	}

	assert(parser != nullptr);

	Solver solver(make_shared< Problem >(parser->cb), set);
	SolveResult result = solver.solve();

	cout_com << "Result: " << result << endl;
	if (result == SolveResult::SOLVE_FEASIBLE || result == SolveResult::SOLVE_OPTIMAL)
	{
		cout_stat << "SATISFIABLE" << endl;
		cout_obj << solver.getSettings()->bestObj << endl;
		cout_sol << "";
		for (int i = 0; i < solver.getSettings()->bestSol->size(); i++)
		{
			if ((*solver.getSettings()->bestSol)[i] == 1)
				cout << "x" << i + 1 << " ";
			else
				cout << "-x" << i + 1 << " ";
		}
		cout << endl;
	}
	else
	{
		cout_stat << "UNSATISFIABLE" << endl;
	}

	return 0;
}