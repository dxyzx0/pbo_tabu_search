//
// Created by psw on 5/20/24.
//


#include <iostream>
#include <spdlog/spdlog.h>
#include "HeuristicsRandom.h"
#include "utils.h"

/***********************************************************************************************************************
 * HeuristicsRandom
 **********************************************************************************************************************/
/**
 * @brief Just try different binary x purely random and test if it is a solution.
 * @return
 */


HeurResult HeuristicsRandom::heuristic()
{
	// generate
	IntegerType bestObj = set->bestObj;
	shared_ptr< IntVec > bestSol = set->bestSol;
	HeurResult result = HeurResult::HEUR_NOTFIND;
	spdlog::info("HeuristicsRandom::heuristic()");
	for (int i = 0; i < nTryPerRound; i++)
	{
		// generate random binary x
		// test if it is a solution
		// if yes, return
		long nVar = prob->getNVar();
		shared_ptr< IntVec > x = gen_rnd_vec(nVar, nVar / 2);
		if (prob->isFeasible(*x))
		{
			auto crntObj = prob->getObj(*x);
			if (crntObj < bestObj) // FIXME:
			{
				bestObj = crntObj;
				bestSol = x;
				result = HeurResult::HEUR_FINDBESTSOL;
				cout_sol_com << "Found a better solution with obj = " << bestObj << endl;
				break;
			}
			else
			{
				result = result == HeurResult::HEUR_FINDBESTSOL ? result : HeurResult::HEUR_FINDBADSOL;
			}

		}
		if (i % 100 == 0)
		{
			spdlog::info("HeuristicsRandom::heuristic() i = {}", i);
		}
	}
	spdlog::info("HeuristicsRandom::heuristic() done");

	if (result == HeurResult::HEUR_FINDBESTSOL)
	{
		set->bestObj = bestObj;
		set->bestSol = bestSol;
	}

	return result;
}

HeuristicsRandom::HeuristicsRandom(shared_ptr< Problem > prob, shared_ptr< Settings > settings) : Heuristics(std::move(
	prob), std::move(settings))
{
}
