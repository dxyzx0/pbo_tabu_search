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
	long nTabuListMax = 10;
	long nTabuTenure = 10;
	long nTabuIterMax = 10000;
	unordered_map< long, size_t > tabuList;  // key: hash of move, value: the iteration when the move is allowed
	IntegerType w_num = 0;  // score = Inf + w_num / w_den * obj
	IntegerType w_den = 1;
	InfMeasure infMeasure = InfMeasure::objRInf; // 0: nInf, 1: rInf, 2: objRInf
	long topkmin = 10;
	long topkdiv = 100;
	bool randomStart = false;
 public:
	HeuristicsTabuSearch(shared_ptr< Problem > prob, shared_ptr< Settings > settings);
	HeurResult heuristic() override;

	tuple< shared_ptr< IntVec >, shared_ptr< IntVec > > calAxb(IntVec& x);

	IntegerType score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq) const;
	IntegerType score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i);
	tuple< IntegerType, shared_ptr< IntVec >, shared_ptr< IntVec > >
	score_with_info(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i);

};

#endif //PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
