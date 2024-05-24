//
// Created by psw on 5/21/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
#define PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_

#include "HeuristicsRandom.h"

#include <algorithm>

using namespace std;

class HeuristicsTabuSearch : public HeuristicsRandom
{
	 protected:
	 long nTabuListMax = 10;
	 long nTabuTenure = 10;
	 unordered_map< long, size_t > tabuList;  // key: hash of move, value: the iteration when the move is allowed
	 IntegerType w_den = 0;
	 IntegerType w_num = 1;
 public:
	HeurResult heuristic() override;
	tuple< shared_ptr< IntVec >, shared_ptr< IntVec > > calAxb(IntVec& x);
	bool isFeasible(IntVec& Ax_b_ineq, IntVec& Ax_b_eq);
	shared_ptr< IntVec > score(IntVec& x, IntVec& Ax);
};

#endif //PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
