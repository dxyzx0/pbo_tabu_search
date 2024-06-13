//
// Created by psw on 5/21/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
#define PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_

#include "HeuristicsRandom.h"

#include <algorithm>
#include <mutex>

using namespace std;

enum class InfMeasure
{
	negNInf,
	negRInf,
	negObjNInf,
	negObjRInf,
};

enum class VarScore
{
	negNInf,
	negRInf,
	negObjNInf,
	negObjRInf,
	normedNegRInf,
};

class HeuristicsTabuSearch : public HeuristicsRandom
{
 protected:
	// Tabu search parameters
	long nTabuTenure = 10;
	long nTabuIterMax = 100000;
	unordered_map< long, size_t > tabuList;  // key: hash of move, value: the iteration when the move is allowed

	// random start or all zero start
	bool randomStart = false;

	// infeasibility measure of solution: 0: nInf, 1: rInf, 2: objNInf, 3: objRInf
	InfMeasure infMeasure = InfMeasure::negRInf;
	// score of variable: 0: nInf, 1: rInf, 2: objNInf, 3: objRInf, 4: normedRInf
	VarScore varScore = VarScore::negRInf;

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

	// mutex for
	std::mutex mtx;
 public:
	HeuristicsTabuSearch(shared_ptr< Problem > prob, shared_ptr< Settings > settings);
	HeurResult heuristic() override;

	void initTabuList();
	void updateTabuList(long i, long t);
	bool varIsNotTabu(long i, long t);

	tuple< shared_ptr< IntVec >, shared_ptr< IntVec > > calAxb(IntVec& x);

	IntegerType score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq) const;
	IntegerType score(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i);
	tuple< IntegerType, shared_ptr< IntVec >, shared_ptr< IntVec > >
	score_with_info(const IntVec& x, const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, long& i);

	void initWeight();
	void updateWeight();

	void testFeasAndUpdateBest(const shared_ptr< IntVec >& Ax_b_ineq,
		const shared_ptr< IntVec >& Ax_b_eq,
		const shared_ptr< IntVec >& x,
		HeurResult& res,
		IntegerType& bestObj);
	void randomSelectMoveFromTopk(shared_ptr< IntVec >& Ax_b_ineq,
		shared_ptr< IntVec >& Ax_b_eq,
		shared_ptr< IntVec >& x,
		IntegerType& score_x,
		IntegerType& best_score,
		IntegerType& best_score_j,
		long& nConsecutiveNonImproving,
		long& best_k,
		long& j,
		size_t seed);

};

#endif //PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
