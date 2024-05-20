//
// Created by psw on 5/20/24.
//

#include "Solver.h"
#include "PresolverNaive.h"

#include <utility>

Solver::Solver(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
{
	ori_prob = std::move(prob);
	set = std::move(settings);

	presolvers.push_back(make_shared< PresolverNaive >(ori_prob, set));
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
	// solve
	// postsolve
	for (auto& presolver : presolvers)
	{
		presolver->postsolve();
	}

}
