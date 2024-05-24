//
// Created by psw on 5/20/24.
//

#include <random>
#include "HeuristicsRandom.h"
#include <iostream>
#include <spdlog/spdlog.h>
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
				cout << "Found a better solution with obj = " << bestObj << endl;
				break;
			}
			else
			{
				result = result == HeurResult::HEUR_FINDBESTSOL ? result : HeurResult::HEUR_FINDBADSOL;
//				cout << "Found a feasible solution with obj = " << crntObj << endl;
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

shared_ptr< IntVec > HeuristicsRandom::gen_rnd_vec(long nVar, long nNonZero, shared_ptr< IntVec > x)
{
	// This function filps nNonZero random elements of a binary vector of size nVar
	// If x is nullptr, a new zero vector is created, then it is filled with nNonZero random ones
	// If x is not nullptr, nNonZeros variables are flipped
	const long N = nVar;    // Total number of elements in the SparseVector
	const long numOnes = nNonZero; // Number of ones to insert

	if (x == nullptr)
	{
		x = make_shared< IntVec >(N);
	}
	else
	{
		assert(x->size() == N);
	}

	// Create a vector to hold indices from 0 to N-1
	std::vector< int > indices(N);
	std::iota(indices.begin(), indices.end(), 0);

	// Create a random number generator
	std::random_device rd;
	std::mt19937 g(rd());

	// Shuffle the indices
	std::shuffle(indices.begin(), indices.end(), g);

	// get an array with indices filled with 1
	for (int i = 0; i < numOnes; ++i)
	{
		(*x)[indices[i]] = 1 - (*x)[indices[i]];
	}
	return x;
}

shared_ptr< IntSpVec > HeuristicsRandom::gen_rnd_spvec(long nVar, long nNonZero)
{
	auto vec = HeuristicsRandom::gen_rnd_vec(nVar, nNonZero);
	auto spVec = make_shared< IntSpVec >(vec->sparseView());

	return spVec;
}

