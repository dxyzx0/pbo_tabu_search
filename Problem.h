//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__PROBLEM_H_
#define PBO_HEURISTICS__PROBLEM_H_

#include <memory>
#include "type.h"
#include "parser/AbcCallback.h"

using namespace std;

/****************************************************************************************
 * Problem class
 ****************************************************************************************/
/**
 * Assume the problem is in the form of
 * min c^T x
 * s.t. A_eq x == b_eq
 *     A_ineq x >= b_ineq
 *     x \in {0, 1}^n
 */
class Problem
{
 private:
	shared_ptr< IntSpMat > A_eq;
	shared_ptr< IntSpMat > A_ineq;
	shared_ptr< IntSpVec > b_eq;
	shared_ptr< IntSpVec > b_ineq;
	shared_ptr< IntSpVec > c;

	long nVar;
	long nCons;
	long nConsEq;
	long nConsIneq;
	long nnz;
	long nnz_A_eq;
	long nnz_A_ineq;
 public:
	explicit Problem(AbcCallback& abcCallback);
	shared_ptr< IntSpMat > getA_eq()
	{
		return A_eq;
	}
	shared_ptr< IntSpMat > getA_ineq()
	{
		return A_ineq;
	}
	shared_ptr< IntSpVec > getB_eq()
	{
		return b_eq;
	}
	shared_ptr< IntSpVec > getB_ineq()
	{
		return b_ineq;
	}
	shared_ptr< IntSpVec > getC()
	{
		return c;
	}
	long getNVar()
	{
		return nVar;
	}
	void setNVar(long nVar)
	{
		this->nVar = nVar;
	}
	long getNCons()
	{
		return nCons;
	}
	void setNCons(long nCons)
	{
		this->nCons = nCons;
	}
	long getNConsEq()
	{
		return nConsEq;
	}
	void setNConsEq(long nConsEq)
	{
		this->nConsEq = nConsEq;
	}
	long getNConsIneq()
	{
		return nConsIneq;
	}
	void setNConsIneq(long nConsIneq)
	{
		this->nConsIneq = nConsIneq;
	}
	bool isFeasible(const IntSpVec& x);
//	bool isFeasible(const IntSpMat& X);
	IntegerType getObj(const IntSpVec& x);
	// copy constructor
	Problem(const Problem& prob);
	// copy assignment
	Problem& operator=(const Problem& prob);
};

#endif //PBO_HEURISTICS__PROBLEM_H_
