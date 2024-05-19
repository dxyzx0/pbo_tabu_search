//
// Created by psw on 5/19/24.
//

#include "Problem.h"
#include <cassert>

using namespace std;

Problem::Problem(AbcCallback& abcCallback)
{
	nVar = abcCallback.getNVar();
	nCons = abcCallback.getNCons();
	nConsEq = 0;
	nConsIneq = 0;
	nnz = 0;
	nnz_A_eq = 0;
	nnz_A_ineq = 0;

	assert (nCons == abcCallback.getRelOp().size() && nCons == abcCallback.getB().size());

	// initialize A_eq, A_ineq, b_eq, b_ineq, c
	A_eq = make_shared< IntSpMat >(nCons, nVar);
	A_ineq = make_shared< IntSpMat >(nCons, nVar);
	b_eq = make_shared< IntSpVec >(nCons);
	b_ineq = make_shared< IntSpVec >(nCons);
	c = make_shared< IntSpVec >(nVar);

	// update b_eq, b_ineq
	std::map< long, string > relOp_map;
	for (auto i = 0; i < abcCallback.getB().size(); i++)
	{
		auto [iConsRelOp, relop] = abcCallback.getRelOp()[i];
		auto [iConsB, b] = abcCallback.getB()[i];

		assert (iConsRelOp == iConsB);

		auto iCons = iConsRelOp;

		relOp_map[iCons] = relop;
		if (relop == "=")
		{
			nConsEq += 1;
			b_eq->insert(iCons) = b;
		}
		else
		{
			assert(relop == ">=");
			nConsIneq += 1;
			b_ineq->insert(iCons) = b;
		}
	}

	for (auto i = 0; i < abcCallback.getA().size(); i++)
	{
		// update stats
		auto [iCons, idVar, coeff] = abcCallback.getA()[i];

		assert (idVar >= 0 && idVar < nVar);
		assert (iCons >= 0 && iCons < nCons);

		nnz += 1;

		auto relop = relOp_map.at(iCons);

		if (relop == "=")
		{
			nnz_A_eq += 1;
			A_eq->insert(iCons, idVar) = coeff;
		}
		else
		{
			assert(relop == ">=");
			nnz_A_ineq += 1;
			A_ineq->insert(iCons, idVar) = coeff;
		}
	}

	for (auto i = 0; i < abcCallback.getC().size(); i++)
	{
		auto [idVar, coeff] = abcCallback.getC()[i];
		c->insert(idVar) = coeff;
	}

	assert(nConsEq + nConsIneq == nCons);
	assert(nnz_A_eq + nnz_A_ineq == nnz);

}

Problem::Problem(const Problem& prob)
{
	nVar = prob.nVar;
	nCons = prob.nCons;
	nConsEq = prob.nConsEq;
	nConsIneq = prob.nConsIneq;
	nnz = prob.nnz;
	nnz_A_eq = prob.nnz_A_eq;
	nnz_A_ineq = prob.nnz_A_ineq;

	A_eq = make_shared< IntSpMat >(*prob.A_eq);
	A_ineq = make_shared< IntSpMat >(*prob.A_ineq);
	b_eq = make_shared< IntSpVec >(*prob.b_eq);
	b_ineq = make_shared< IntSpVec >(*prob.b_ineq);
	c = make_shared< IntSpVec >(*prob.c);
}

Problem& Problem::operator=(const Problem& prob)
{
	if (this == &prob)
	{
		return *this;
	}
	nVar = prob.nVar;
	nCons = prob.nCons;
	nConsEq = prob.nConsEq;
	nConsIneq = prob.nConsIneq;
	nnz = prob.nnz;
	nnz_A_eq = prob.nnz_A_eq;
	nnz_A_ineq = prob.nnz_A_ineq;

	A_eq = make_shared< IntSpMat >(*prob.A_eq);
	A_ineq = make_shared< IntSpMat >(*prob.A_ineq);
	b_eq = make_shared< IntSpVec >(*prob.b_eq);
	b_ineq = make_shared< IntSpVec >(*prob.b_ineq);
	c = make_shared< IntSpVec >(*prob.c);

	return *this;
}
