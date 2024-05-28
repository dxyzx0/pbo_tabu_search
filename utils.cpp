//
// Created by psw on 5/28/24.
//

#include <memory>
#include <ranges>
#include <random>
#include "utils.h"

bool isFeasible(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq)
{
	// test if Ax_b_ineq >= 0 and Ax_b_eq == 0
	if (!std::ranges::all_of(Ax_b_ineq, [](const IntegerType& i)
	{ return i >= 0; }))
	{
		return false;
	}
	if (!std::ranges::all_of(Ax_b_eq, [](const IntegerType& i)
	{ return i == 0; }))
	{
		return false;
	}
	return true;
}

IntegerType nInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq)
{
	IntegerType infeasibility = 0;
	for (const IntegerType& i : Ax_b_ineq)
	{
		infeasibility += i < 0 ? 1 : 0;
	}
	for (const IntegerType& i : Ax_b_eq)
	{
		infeasibility += i != 0 ? 1 : 0;
	}
	return infeasibility;
}

IntegerType rInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq)
{
	IntegerType infeasibility = 0;
	for (const IntegerType& i : Ax_b_ineq)
	{
		infeasibility += i < 0 ? -i : IntegerType(0);
	}
	for (const IntegerType& i : Ax_b_eq)
	{
		infeasibility += i > 0 ? i : -i;
	}
	return infeasibility;
}

std::shared_ptr< IntVec > gen_rnd_vec(long nVar, long nNonZero, std::shared_ptr< IntVec > x)
{
	// This function filps nNonZero random elements of a binary vector of size nVar
	// If x is nullptr, a new zero vector is created, then it is filled with nNonZero random ones
	// If x is not nullptr, nNonZeros variables are flipped
	const long N = nVar;    // Total number of elements in the SparseVector
	const long numOnes = nNonZero; // Number of ones to insert

	if (x == nullptr)
	{
		x = std::make_shared< IntVec >(N);
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

std::shared_ptr< IntSpVec > gen_rnd_spvec(long nVar, long nNonZero)
{
	auto vec = gen_rnd_vec(nVar, nNonZero, nullptr);
	auto spVec = std::make_shared< IntSpVec >(vec->sparseView());

	return spVec;
}
