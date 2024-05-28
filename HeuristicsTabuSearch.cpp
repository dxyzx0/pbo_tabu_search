//
// Created by psw on 5/21/24.
//

#include <ranges>
#include <spdlog/spdlog.h>
#include <queue>
#include "HeuristicsTabuSearch.h"
#include "utils.h"

void HeuristicsTabuSearch::initTabuList()
{
	for (long i = 0; i < prob->getNVar(); i++) tabuList[i] = 0;
}

void HeuristicsTabuSearch::updateTabuList(long i, long t)
{
	tabuList[i] = t + nTabuTenure;
}

bool HeuristicsTabuSearch::varIsNotTabu(long i, long t)
{
	return tabuList[i] <= t;
}

void HeuristicsTabuSearch::initWeight()
{
	w_num = w_num_init;
	w_den = w_den_init;
}

void HeuristicsTabuSearch::updateWeight()
{
	w_num = nBestSolFound;
	w_den = 1;
}

HeurResult HeuristicsTabuSearch::heuristic()
{
	IntegerType bestObj = set->bestObj;
	shared_ptr< IntVec > bestSol = set->bestSol;
	HeurResult res = HeurResult::HEUR_NOTFIND;
	for (long i = 0; i < nTryPerRound; i++)
	{
		spdlog::info("(*) Tabu search Outer iteration {} with bestObj = {}", i, bestObj.get_str());

		// init weight for objective function
		initWeight();

		long nVar = prob->getNVar();

		shared_ptr< IntVec > x = randomStart ? gen_rnd_vec(nVar, nVar / 2) : make_shared< IntVec >(nVar);

		auto [Ax_b_ineq, Ax_b_eq] = calAxb(*x);
		IntegerType score_x = score(*x, *Ax_b_ineq, *Ax_b_eq);

		IntegerType best_score = score_x;

		// record consecutive non-improving iterations
		long nConsecutiveNonImproving = 0;

		spdlog::info("(*) Initial score = {}", score_x.get_str());
		for (long j = 0; j < nTabuIterMax; j++)
		{
			spdlog::info("(**) Tabu search iteration {}, score = {}, best score = {}",
				j, score_x.get_str(), best_score.get_str());

			// test feasibility and update best solution
			testFeasAndUpdateBest(Ax_b_ineq, Ax_b_eq, x, res, bestObj);

			// init
			IntegerType best_score_j = score_x;
			long best_k = -1;

			assert (*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
			assert (*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

			randomSelectMoveFromTopk(Ax_b_ineq,
				Ax_b_eq,
				x,
				score_x,
				best_score,
				best_score_j,
				nConsecutiveNonImproving,
				best_k,
				j);

			assert (*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
			assert (*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

			// update weight for objective function
			updateWeight();
		}
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

IntegerType HeuristicsTabuSearch::score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i)
{
	assert(Ax_b_ineq == (*prob->getA_ineq()) * x - (*prob->getB_ineq()));
	assert(Ax_b_eq == (*prob->getA_eq()) * x - (*prob->getB_eq()));

	auto col_ineq_i = prob->getA_ineq()->col(i) * (1 - 2 * (x[i] == 1));
	auto col_eq_i = prob->getA_eq()->col(i) * (1 - 2 * (x[i] == 1));
	auto Ax_b_ineq_i = (Ax_b_ineq + col_ineq_i).toDense();
	auto Ax_b_eq_i = (Ax_b_eq + col_eq_i).toDense();
	if (varScore == VarScore::negNInf)
		return -nInf(Ax_b_ineq_i, Ax_b_eq_i);
	else if (varScore == VarScore::negRInf)
		return -rInf(Ax_b_ineq_i, Ax_b_eq_i);
	else if (varScore == VarScore::negObjNInf)
	{
		IntegerType obj = prob->getObj(x);
		return -nInf(Ax_b_ineq_i, Ax_b_eq_i) - w_num * obj / w_den;
	}
	else if (varScore == VarScore::negObjRInf)
	{
		IntegerType obj = prob->getObj(x);
		return -rInf(Ax_b_ineq_i, Ax_b_eq_i) - w_num * obj / w_den;
	}
	else if (varScore == VarScore::normedNegRInf)
	{
		shared_ptr< IntSpMat > A_ineq = prob->getA_ineq();
		shared_ptr< IntSpMat > A_eq = prob->getA_eq();
		auto Ax_ineq = Ax_b_ineq + *prob->getB_ineq();
		auto Ax_eq = Ax_b_eq + *prob->getB_eq();
//		auto Ax_ineq_test = (*A_ineq) * x;
//		auto Ax_eq_test = (*A_eq) * x;
//
//		for (size_t i = 0; i < Ax_ineq.size(); i++)
//		{
//			if (Ax_ineq[i] != Ax_ineq_test[i]) {
//				cout << "i=" << i << "Ax_ineq[i] = " << Ax_ineq[i].get_str() << ", Ax_ineq_test[i] = " << Ax_ineq_test[i].get_str() << endl;
//			}
//		}
//		for (size_t i = 0; i < Ax_eq.size(); i++)
//		{
//			if (Ax_eq[i] != Ax_eq_test[i]) {
//				cout << "i=" << i << " Ax_eq[i] = " << Ax_eq[i].get_str() << ", Ax_eq_test[i] = " << Ax_eq_test[i].get_str() << endl;
//			}
//		}
		return -normedRInf(Ax_b_ineq_i, Ax_b_eq_i, Ax_ineq, Ax_eq);
	}
	else
		throw std::runtime_error("Invalid infMeasure");
}

IntegerType HeuristicsTabuSearch::score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq) const
{
	assert(Ax_b_ineq == (*prob->getA_ineq()) * x - (*prob->getB_ineq()));
	assert(Ax_b_eq == (*prob->getA_eq()) * x - (*prob->getB_eq()));

	if (infMeasure == InfMeasure::negNInf)
		return -nInf(Ax_b_ineq, Ax_b_eq);
	else if (infMeasure == InfMeasure::negRInf)
		return -rInf(Ax_b_ineq, Ax_b_eq);
	else if (infMeasure == InfMeasure::negObjNInf)
	{
		IntegerType obj = prob->getObj(x);
		return -nInf(Ax_b_ineq, Ax_b_eq) - w_num * obj / w_den;
	}
	else if (infMeasure == InfMeasure::negObjRInf)
	{
		IntegerType obj = prob->getObj(x);
		return -rInf(Ax_b_ineq, Ax_b_eq) - w_num * obj / w_den;
	}
	else
		throw std::runtime_error("Invalid infMeasure");
}

tuple< IntegerType, shared_ptr< IntVec >, shared_ptr< IntVec > >
HeuristicsTabuSearch::score_with_info(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i)
{
	assert(Ax_b_ineq == (*prob->getA_ineq()) * x - (*prob->getB_ineq()));
	assert(Ax_b_eq == (*prob->getA_eq()) * x - (*prob->getB_eq()));

	auto col_ineq_i = prob->getA_ineq()->col(i) * (1 - 2 * (x[i] == 1));
	auto col_eq_i = prob->getA_eq()->col(i) * (1 - 2 * (x[i] == 1));
	IntegerType score_i = score(x, Ax_b_ineq, Ax_b_eq, i);
	auto Ax_b_ineq_i = make_shared< IntVec >(std::move((Ax_b_ineq + col_ineq_i).toDense()));
	auto Ax_b_eq_i = make_shared< IntVec >(std::move((Ax_b_eq + col_eq_i).toDense()));
	return make_tuple(score_i, Ax_b_ineq_i, Ax_b_eq_i);
}

HeuristicsTabuSearch::HeuristicsTabuSearch(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
	: HeuristicsRandom(
	prob, settings)
{
	initTabuList();
}

void HeuristicsTabuSearch::testFeasAndUpdateBest(const shared_ptr< IntVec >& Ax_b_ineq,
	const shared_ptr< IntVec >& Ax_b_eq,
	const shared_ptr< IntVec >& x,
	HeurResult& res,
	IntegerType& bestObj)
{
	assert (*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
	assert (*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	if (isFeasible(*Ax_b_ineq, *Ax_b_eq))
	{
//				auto [Ax_b_ineq_test, Ax_b_eq_test] = calAxb(*x);
//				test_isFeasible(*Ax_b_ineq_test, *Ax_b_eq_test, *Ax_b_ineq, *Ax_b_eq);
		nSolFound++;  // record the number of solutions found
		IntegerType crntObj = prob->getObj(*x);
		spdlog::info("(**) Found a feasible solution with obj = {}", crntObj.get_str());
		if (crntObj < bestObj)
		{
			nBestSolFound++;  // record the number of best solutions found
			// format solution into a string
			std::stringstream ss;
			for (long l = 0; l < prob->getNVar(); l++)
			{
				ss << (*x)[l].get_str() << " ";
			}
			std::string solStr = ss.str();
			spdlog::info("(**) Feasible solution found: {}", solStr);

			spdlog::info("(**) Found a better solution with obj = {}", crntObj.get_str());
			set->bestObj = bestObj = crntObj;
			// update best solution using a copy of x
			set->bestSol = make_shared< IntVec >(*x);
			res = HeurResult::HEUR_FINDBESTSOL;
			cout << "Found a better solution with obj = " << bestObj << endl;
		}
		else
		{
			res = res == HeurResult::HEUR_FINDBESTSOL ? res : HeurResult::HEUR_FINDBADSOL;
		}
	}
}

void HeuristicsTabuSearch::randomSelectMoveFromTopk(shared_ptr< IntVec >& Ax_b_ineq,
	shared_ptr< IntVec >& Ax_b_eq,
	shared_ptr< IntVec >& x,
	IntegerType& score_x,
	IntegerType& best_score,
	IntegerType& best_score_j,
	long& nConsecutiveNonImproving,
	long& best_k,
	long& j)
{
	assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
	assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	long nVar = prob->getNVar();
	long max_k = max(nVar / topkdiv, topkmin);

	// random select a move from the top k moves
	// Define a priority queue to store the top k scores and their indices
	auto comp = [](const std::tuple< IntegerType, long, shared_ptr< IntVec >, shared_ptr< IntVec > >& a,
		std::tuple< IntegerType, long, shared_ptr< IntVec >, shared_ptr< IntVec > >& b)
	{
	  return std::get< 0 >(a) > std::get< 0 >(b);
	};
	std::priority_queue< std::tuple< IntegerType, long, shared_ptr< IntVec >, shared_ptr< IntVec > >,
						 std::vector< std::tuple< IntegerType,
												  long,
												  shared_ptr< IntVec >,
												  shared_ptr< IntVec > > >,
						 decltype(comp) > topk_scores;

	for (long k = 0; k < nVar; k++)
	{
		auto [score_k, Ax_b_ineq_k, Ax_b_eq_k] = score_with_info(*x, *Ax_b_ineq, *Ax_b_eq, k);
		if ((score_k > best_score_j && varIsNotTabu(k, j)) || (score_k > best_score))
		{  // greedy
			// Add the score and index to the priority queue
			topk_scores.emplace(score_k, k, Ax_b_ineq_k, Ax_b_eq_k);
			// If the size of the queue exceeds k, remove the smallest element
			if (topk_scores.size() > max_k)
			{
				topk_scores.pop();
			}
		}
	}
	assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
	assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	// If the size of the queue is less than k, set k to the size of the queue
	size_t rand_k = !topk_scores.empty() ? topk_scores.size() : 0;
	while (rand_k-- > 0)
	{
		auto [score_k, k, Ax_b_ineq_k, Ax_b_eq_k] = topk_scores.top();
		if (rand_k == 0)
		{
			best_score_j = score_k;
			best_k = k;
//					auto [score_k_test, Ax_b_ineq_k, Ax_b_eq_k] = score_with_info(*x, *Ax_b_ineq, *Ax_b_eq, k);
			Ax_b_ineq = Ax_b_ineq_k;
			Ax_b_eq = Ax_b_eq_k;
//					assert(score_k == score_k_test);

			if (best_score_j > best_score)
			{
				spdlog::info("(**) Found a new best local candidate with score = {}", best_score_j.get_str());
				best_score = best_score_j;
				nConsecutiveNonImproving = 0;
			}
		}
	}
	if (best_k != -1)
	{
		spdlog::info("(**) Found a better move {} with score = {}", best_k, best_score_j.get_str());
		(*x)[best_k] = 1 - (*x)[best_k];

		updateTabuList(best_k, j);
		score_x = score(*x, *Ax_b_ineq, *Ax_b_eq);

		assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
		assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	}
	else
	{
		// randomly select a move
		nConsecutiveNonImproving++;
		long nFlip = min(nVar / 10 * nConsecutiveNonImproving, nVar * 2 / 3);
		spdlog::info("(**) No better move found in {} consecutive iterations, randomly flip {} vars",
			nConsecutiveNonImproving, nFlip);
		x = gen_rnd_vec(nVar, nFlip, x);
		tie(Ax_b_ineq, Ax_b_eq) = calAxb(*x);
		score_x = score(*x, *Ax_b_ineq, *Ax_b_eq);
		spdlog::info("(**) Randomly flipped x with score = {}", score_x.get_str());

		assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
		assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	}
}