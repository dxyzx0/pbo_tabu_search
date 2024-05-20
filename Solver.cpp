//
// Created by psw on 5/20/24.
//

#include "Solver.h"
#include "PresolverNaive.h"
#include "HeuristicsRandom.h"

#include <memory>

using namespace std;

Solver::Solver(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
{
	ori_prob = std::move(prob);
	set = std::move(settings);
}

void Solver::addDefaultPlugins()
{
	presolvers.push_back(make_shared< PresolverNaive >(tran_prob, set));
	heuristics.push_back(make_shared< HeuristicsRandom >(tran_prob, set));
}

void Solver::initSolver()
{
	tran_prob = make_shared< Problem >(*ori_prob);  // FIXME: check whether this is a deep copy
	addDefaultPlugins();
}

SolveResult Solver::solve()
{
	Stage solveStage = Stage::INITSOLVER;
	initSolver();
	solveStage = Stage::SOLVERINITED;

	SolveResult result = SolveResult::SOLVE_UNKNOWN;

	// presolve
	solveStage = Stage::INITPRESOLVE;
	PresResult presResult = presolve(result);
	if (result == SolveResult::SOLVE_INFEASIBLE)
	{
		return result;
	}
	solveStage = Stage::PRESOLVED;

	// main loop
	solveStage = Stage::SOLVING;
	long iter = 0;
	while (iter < set->maxIter)
	{
		// heuristics
		for (auto& heur : heuristics)
		{
			heur->heuristic();
		}
	}

	// postsolve
	solveStage = Stage::INITPOSTSOLVE;
	for (auto& presolver : presolvers)
	{
		presolver->postsolve();
	}

	if (set->bestObj < numeric_limits< IntegerType >::max())
	{
		result = SolveResult::SOLVE_FEASIBLE;
	}
	else
	{
		result = SolveResult::SOLVE_UNKNOWN;
	}
	solveStage = Stage::POSTSOLVED;

	return result;
}

PresResult Solver::presolve(SolveResult& result)
{
	PresResult presResult = PresResult::PRES_UNCHANGED;
	for (auto& presolver : presolvers)
	{
		PresResult currentPresResult = presolver->presolve();
		if (currentPresResult == PresResult::PRES_REDUCED)
		{
			presResult = PresResult::PRES_REDUCED;
		}
		else if (currentPresResult == PresResult::PRES_INFEASIBLE)
		{
			result = SolveResult::SOLVE_INFEASIBLE;
		}
	}
	return presResult;
}

