//
// Created by psw on 5/21/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
#define PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_

#include "HeuristicsRandom.h"

#include <algorithm>

using namespace std;

class HeuristicsTabuSearch : public HeuristicsRandom {
protected:
    long nTabuListMax = 10;
    long nTabuTenure = 10;
    long nTabuIterMax = 10000;
    unordered_map<long, size_t> tabuList;  // key: hash of move, value: the iteration when the move is allowed
    IntegerType w_den = 0;
    IntegerType w_num = 1;
	int infMeasure = 1; // 0: nInf, 1: rInf
	long topkmin = 10;
	long topkdiv = 100;
	bool randomStart = false;
public:
    HeuristicsTabuSearch(shared_ptr<Problem> prob, shared_ptr<Settings> settings);
    HeurResult heuristic() override;

    tuple<shared_ptr<IntVec>, shared_ptr<IntVec> > calAxb(IntVec &x);

    bool isFeasible(IntVec &Ax_b_ineq, IntVec &Ax_b_eq);

    IntegerType nInf(IntVec &Ax_b_ineq, IntVec &Ax_b_eq);
	IntegerType rInf(IntVec &Ax_b_ineq, IntVec &Ax_b_eq);

    IntegerType score(long &i, IntVec &x, IntVec &Ax_b_ineq, IntVec &Ax_b_eq);

    tuple<IntegerType, shared_ptr<IntVec>, shared_ptr<IntVec> >
    score_with_info(long &i, IntVec &x, IntVec &Ax_b_ineq, IntVec &Ax_b_eq);

    IntegerType score(IntVec &Ax_b_ineq, IntVec &Ax_b_eq);
};

#endif //PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
