//
// Created by psw on 5/21/24.
//

#include "HeuristicsTabuSearch.h"
#include <ranges>

HeurResult HeuristicsTabuSearch::heuristic()
{
	HeurResult res = HeurResult::HEUR_NOTFIND;
	for (long i = 0; i < nTryPerRound; i++)
	{
//		shared_ptr< IntVec > move = gen_rnd_vec(prob->nVar, 2);
//		long hash = move->at(0) * prob->nVar + move->at(1);
//		if (tabuList.find(hash) == tabuList.end() || tabuList[hash] < i)
//		{
//			HeurResult res = prob->evaluate_move(move);
//			if (res.obj < prob->objVal)
//			{
//				prob->update_move(move);
//				prob->objVal = res.obj;
//				tabuList[hash] = i + nTabuTenure;
//			}
//		}
	}

	return res;
}

tuple< shared_ptr< IntVec >, shared_ptr< IntVec > > HeuristicsTabuSearch::calAxb(IntVec& x)
{
	auto A_ineq = prob->getA_ineq();
	auto A_eq = prob->getA_eq();

	auto b_ineq = prob->getB_ineq();
	auto b_eq = prob->getB_eq();

	auto Ax_b_ineq = make_shared< IntVec >((*A_ineq) * x - (*b_ineq));
	auto Ax_b_eq = make_shared< IntVec >((*A_eq) * x - (*b_eq));

	return make_tuple(Ax_b_ineq, Ax_b_eq);
}

bool HeuristicsTabuSearch::isFeasible(IntVec& Ax_b_ineq, IntVec& Ax_b_eq)
{
	// test if Ax_b_ineq >= 0 and Ax_b_eq == 0
	if (!ranges::all_of(Ax_b_eq, [](auto i) { return i == 0; }))
	{
		return false;
	}
	if (!ranges::all_of(Ax_b_ineq, [](auto i) { return i >= 0; }))
	{
		return false;
	}
	return true;
}
