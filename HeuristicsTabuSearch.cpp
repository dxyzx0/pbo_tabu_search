//
// Created by psw on 5/21/24.
//

#include <ranges>
#include <queue>
#include <thread>
#include <random>
#include <spdlog/spdlog.h>
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
	std::vector< std::thread > threads; // Vector to hold the threads
	threads.reserve(set->numthreads);

	for (size_t i = 0; i < set->numthreads; i++)
	{
		threads.emplace_back([&, i]()
		{ // Capture variables by reference and i by value
		  spdlog::info("(*) Tabu search Outer iteration {} with bestObj = {}", i, to_string(bestObj));
		  size_t seed = i;

		  // init weight for objective function
		  initWeight();

		  long nVar = prob->getNVar();

		  shared_ptr< IntVec > x = randomStart ? gen_rnd_vec(nVar, nVar / 2) : make_shared< IntVec >(nVar);

		  auto [Ax_b_ineq, Ax_b_eq] = calAxb(*x);
		  IntegerType score_x = score(*x, *Ax_b_ineq, *Ax_b_eq);

		  IntegerType best_score = score_x;

		  // record consecutive non-improving iterations
		  long nConsecutiveNonImproving = 0;

		  spdlog::info("(*) Initial score = {}", to_string(score_x));
		  for (long j = 0; j < nTabuIterMax; j++)
		  {
			  // break if time limit is reached
			  double time = std::chrono::duration_cast< std::chrono::milliseconds >(
				  std::chrono::steady_clock::now() - set->startTime).count() / 1000.0;
			  if (time > set->timelimit)
			  {
				  spdlog::info("(*) Tabu search time limit reached");
				  break;
			  }

			  if (j % 1000 == 0)
			  {
				  spdlog::info("(**) Tabu search iteration {}, score = {}, best score = {}",
					  j, to_string(score_x), to_string(best_score));
			  }

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
				  j,
				  seed);

			  assert (*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
			  assert (*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

			  // update weight for objective function
			  updateWeight();
		  }
		});
	}

	// Join the threads
	for (auto& thread : threads)
	{
		thread.join();
	}

	if (res == HeurResult::HEUR_FINDBESTSOL)
	{
		spdlog::info("(*) Tabu search found a feasible solution with obj = {}", to_string(bestObj));
	}
	else
	{
		spdlog::info("(*) Tabu search found no solution.");
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
	: HeuristicsRandom(prob, settings)
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
		spdlog::info("(**) Found a feasible solution with obj = {}", to_string(crntObj));
		if (crntObj < bestObj)
		{
			nBestSolFound++;  // record the number of best solutions found
			// format solution into a string
			std::stringstream ss;
			for (long l = 0; l < prob->getNVar(); l++)
			{
				ss << to_string((*x)[l]) << " ";
			}
			std::string solStr = ss.str();
			spdlog::info("(**) Feasible solution found: {}", solStr);

			spdlog::info("(**) Found a better solution with obj = {}", to_string(crntObj));
			{
				std::lock_guard< std::mutex > lock(mtx); // Lock the mutex for shared data
				set->bestObj = bestObj = crntObj;
				// update best solution using a copy of x
				set->bestSol = make_shared< IntVec >(*x);
				res = HeurResult::HEUR_FINDBESTSOL;
			}

			cout_sol_com << "Found a better solution with obj = " << bestObj << endl;
		}
		else
		{
			{
				std::lock_guard< std::mutex > lock(mtx); // Lock the mutex for shared data
				res = res == HeurResult::HEUR_FINDBESTSOL ? res : HeurResult::HEUR_FINDBADSOL;
			}
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
	long& j,
	size_t seed)
{
	assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
	assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	long nVar = prob->getNVar();
	size_t max_k = max(nVar / topkdiv, topkmin);
	best_k = -1;

	// random select a move from the top k moves
	// Define a priority queue to store the top k scores and their indices
	auto comp = [](const std::tuple< IntegerType, long, shared_ptr< IntVec >, shared_ptr< IntVec > >& a,
		std::tuple< IntegerType, long, shared_ptr< IntVec >, shared_ptr< IntVec > >& b)
	{
	  return std::get< 0 >(a) < std::get< 0 >(b);
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
		}
	}
	assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
	assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	int topk = min(max_k, topk_scores.size());

	// If the size of the queue is less than k, set k to the size of the queue
	// rank_k is a random number between 0 and topk_scores.size() - 1
	int rand_k;
	if (!topk_scores.empty())
	{
		std::mt19937 gen(seed);
		std::uniform_int_distribution<> dis(0, topk - 1);
		rand_k = dis(gen);
	}
	else
	{
		rand_k = 0;
	}
	spdlog::debug("(**) Randomly selected move {} from top {} moves", rand_k, topk_scores.size());

	while (rand_k-- > 0)
	{
		auto [score_k, k, Ax_b_ineq_k, Ax_b_eq_k] = topk_scores.top();
//		spdlog::info("(**) Top {} move with score = {}", rand_k, to_string(score_k));
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
				spdlog::info("(**) Found a new best local candidate with score = {}", to_string(best_score_j));
				best_score = best_score_j;
				nConsecutiveNonImproving = 0;
			}
		}
	}
	if (best_k != -1)
	{
		spdlog::debug("(**) Found a better move {} with score = {}", best_k, to_string(best_score_j));
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
		spdlog::debug("(**) No better move found in {} consecutive iterations, randomly flip {} vars",
			nConsecutiveNonImproving, nFlip);
		x = gen_rnd_vec(nVar, nFlip, x, time(nullptr));
		tie(Ax_b_ineq, Ax_b_eq) = calAxb(*x);
		score_x = score(*x, *Ax_b_ineq, *Ax_b_eq);
		spdlog::debug("(**) Randomly flipped x with score = {}", to_string(score_x));

		assert(*Ax_b_ineq == (*prob->getA_ineq()) * (*x) - (*prob->getB_ineq()));
		assert(*Ax_b_eq == (*prob->getA_eq()) * (*x) - (*prob->getB_eq()));

	}
}