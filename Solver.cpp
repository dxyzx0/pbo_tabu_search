//
// Created by psw on 5/20/24.
//

#include "Solver.h"
#include "PresolverNaive.h"
#include "HeuristicsRandom.h"

#include <utility>

Solver::Solver(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
{
	ori_prob = std::move(prob);
	set = std::move(settings);

	addDefaultPlugins();
}



SolveResult Solver::solve()
{
	// presolve
	for (auto& presolver : presolvers)
	{
		presolver->presolve();
	}
	// propagate
	for (auto& propgator : propgators)
	{
		propgator->propagate();
	}
	// heuristics
	for (auto& heur : heuristics)
	{
		heur->heuristic();
	}
	// postsolve
	for (auto& presolver : presolvers)
	{
		presolver->postsolve();
	}

}
void Solver::addDefaultPlugins()
{
	presolvers.push_back(make_shared< PresolverNaive >(ori_prob, set));
	heuristics.push_back(make_shared< HeuristicsRandom >(ori_prob, set));
}
