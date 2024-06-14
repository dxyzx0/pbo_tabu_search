//
// Created by psw on 5/28/24.
//

#include <iostream>
#include <memory>
#include <ranges>
#include <random>
#include <cassert>
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

IntegerType normedRInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, const IntVec& Ax_ineq, const IntVec& Ax_eq)
{
	assert(Ax_b_ineq.size() == Ax_ineq.size());
	assert(Ax_b_eq.size() == Ax_eq.size());
	IntegerType infeasibility = 0;
	for (size_t i = 0; i < Ax_b_ineq.size(); i++)
	{
		infeasibility += Ax_b_ineq[i] < 0 && Ax_ineq[i] != 0 ? abs(Ax_b_ineq[i] / Ax_ineq[i]) : IntegerType(0);
	}
	for (size_t i = 0; i < Ax_b_eq.size(); i++)
	{
		infeasibility += Ax_eq[i] != 0 ? abs(Ax_b_eq[i] / Ax_eq[i]) : IntegerType(0);
	}
	return infeasibility;
}

std::shared_ptr< IntVec > gen_rnd_vec(long nVar, long nNonZero, std::shared_ptr< IntVec > x, size_t seed)
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
	auto vec = gen_rnd_vec(nVar, nNonZero, nullptr, time(nullptr));
	auto spVec = std::make_shared< IntSpVec >(vec->sparseView());

	return spVec;
}

std::shared_ptr< IntVec > str2vec(const std::string& str) {
	std::stringstream ss(str);
	std::vector< IntegerType > vec;
	int num;

	int i = 0;
	while (ss >> num) {
		cout_com << "num = " << num << std::endl;
		vec.emplace_back(num);
		i++;
	}
	std::shared_ptr< IntVec > x = std::make_shared< IntVec >(vec.size());
	std::copy(vec.begin(), vec.end(), x->begin());
	return x;
}

bool test_isFeasible(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, const IntVec& Ax_b_ineq_expected, const IntVec& Ax_b_eq_expected)
{
	bool isFeasible = true;
	for (int i = 0; i < Ax_b_ineq.size(); i++)
	{
		if (Ax_b_ineq[i] != Ax_b_ineq_expected[i])
		{
			cout_com << "Ax_b_ineq[" << i << "] = " << Ax_b_ineq[i] << " != " << Ax_b_ineq_expected[i] << std::endl;
			isFeasible = false;
		}
	}
	for (int i = 0; i < Ax_b_eq.size(); i++)
	{
		if (Ax_b_eq[i] != Ax_b_eq_expected[i])
		{
			cout_com << "Ax_b_eq[" << i << "] = " << Ax_b_eq[i] << " != " << Ax_b_eq_expected[i] << std::endl;
			isFeasible = false;
		}
	}
	return isFeasible;
}

#ifdef useGMP
std::string to_string(const mpz_class& x)
{
	return x.get_str();
}
#endif

