//
// Created by psw on 5/21/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
#define PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_

#include "HeuristicsRandom.h"

#include <algorithm>

using namespace std;

enum class InfMeasure
{
	nInf,
	rInf,
	objNInf,
	objRInf,
};

class HeuristicsTabuSearch : public HeuristicsRandom
{
 protected:
	// Tabu search parameters
	long nTabuTenure = 10;
	long nTabuIterMax = 10000;
	unordered_map< long, size_t > tabuList;  // key: hash of move, value: the iteration when the move is allowed

	// random start or all zero start
	bool randomStart = false;

	// infeasibility measure: 0: nInf, 1: rInf, 2: objNInf, 3: objRInf
	InfMeasure infMeasure = InfMeasure::rInf;

	// randomly select a move among topk moves, which is determined by max(topkmin, nVar / topkdiv)
	long topkmin = 10;
	long topkdiv = 100;

	// weight for objective function: score = Inf + w_num / w_den * obj
	IntegerType w_num = 0;
	IntegerType w_den = 1;
	IntegerType w_num_init = 0;
	IntegerType w_den_init = 1;

	// solution
	int nSolFound = 0;
	int nBestSolFound = 0;

 public:
	HeuristicsTabuSearch(shared_ptr< Problem > prob, shared_ptr< Settings > settings);
	HeurResult heuristic() override;

	void initTabuList() { for (long i = 0; i < prob->getNVar(); i++) tabuList[i] = 0; }
	void updateTabuList(long i, long t) { tabuList[i] = t + nTabuTenure; }
	bool varIsNotTabu(long i, long t) { return tabuList[i] <= t; }

	tuple< shared_ptr< IntVec >, shared_ptr< IntVec > > calAxb(IntVec& x);

	IntegerType score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq) const;
	IntegerType score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i);
	tuple< IntegerType, shared_ptr< IntVec >, shared_ptr< IntVec > >
	score_with_info(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i);

	void initWeight() { w_num = w_num_init; w_den = w_den_init; }
	void updateWeight() { w_num = nBestSolFound; w_den = 1;}

};

#endif //PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
