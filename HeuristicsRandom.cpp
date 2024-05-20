//
// Created by psw on 5/20/24.
//

#include <random>
#include "HeuristicsRandom.h"
#include <iostream>
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
	shared_ptr< IntSpVec > bestSol = set->bestSol;
	HeurResult result = HeurResult::HEUR_NOTFIND;
	for (int i = 0; i < nTryPerRound; i++)
	{
		// generate random binary x
		// test if it is a solution
		// if yes, return
		long nVar = prob->getNVar();
		auto x = gen_rnd_spvec(nVar, nVar / 2);
		if (prob->isFeasible(*x))
		{
			auto crntObj = prob->getObj(*x);
			if (crntObj < bestObj) // FIXME:
			{
				bestObj = crntObj;
				bestSol = x;
				result = HeurResult::HEUR_FINDBESTSOL;
				cout << "Found a better solution with obj = " << bestObj << endl;
				break;
			}
			else
			{
				result = result == HeurResult::HEUR_FINDBESTSOL ? result : HeurResult::HEUR_FINDBADSOL;
//				cout << "Found a feasible solution with obj = " << crntObj << endl;
			}

		}
	}

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

shared_ptr< IntSpVec > HeuristicsRandom::gen_rnd_spvec(long nVar, long nNonZero)
{
	IntSpVec x(nVar);
	const int N = nVar;    // Total number of elements in the SparseVector
	const int numOnes = nNonZero; // Number of ones to insert

	// Create a vector to hold indices from 0 to N-1
	std::vector< int > indices(N);
	std::iota(indices.begin(), indices.end(), 0);

	// Create a random number generator
	std::random_device rd;
	std::mt19937 g(rd());

	// Shuffle the indices
	std::shuffle(indices.begin(), indices.end(), g);

	// Create a SparseVector with a size of N
	auto spVec = make_shared< IntSpVec >(nVar);

	// Insert ones at the first numOnes shuffled indices
	for (int i = 0; i < numOnes; ++i)
	{
		spVec->insert(indices[i]) = 1;
	}

	// Finalize the vector setup
	spVec->finalize();
	return spVec;
}
