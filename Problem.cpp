//
// Created by psw on 5/19/24.
//

#include "Problem.h"
#include <cassert>
#include <iostream>

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

	for (const auto& [iConsRelOp, relop]: abcCallback.getRelOp())
	{
		if (relop == "=")
		{
			nConsEq += 1;
		}
		else
		{
			assert(relop == ">=");
			nConsIneq += 1;
		}
	}

	// initialize A_eq, A_ineq, b_eq, b_ineq, c
	A_eq = make_shared< IntSpMat >(nConsEq, nVar);
	A_ineq = make_shared< IntSpMat >(nConsIneq, nVar);
	b_eq = make_shared< IntSpVec >(nConsEq);
	b_ineq = make_shared< IntSpVec >(nConsIneq);
	c = make_shared< IntSpVec >(nVar);

	// update b_eq, b_ineq
	long iCons_eq = 0;
	long iCons_ineq = 0;
	map< long, long > eq_cons_map, ineq_cons_map;
	std::map< long, string > relOp_map;
	for (auto i = 0; i < abcCallback.getB().size(); i++)
	{
		const auto [iConsRelOp, relop] = abcCallback.getRelOp()[i];
		const auto [iConsB, b] = abcCallback.getB()[i];

		assert (iConsRelOp == iConsB);

		auto iCons = iConsRelOp;

		relOp_map[iCons] = relop;
		if (relop == "=")
		{
			b_eq->insert(iCons_eq) = b;
			eq_cons_map[iCons] = iCons_eq;
			iCons_eq++;
		}
		else
		{
			assert(relop == ">=");
			b_ineq->insert(iCons_ineq) = b;
			ineq_cons_map[iCons] = iCons_ineq;
			iCons_ineq++;
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
			auto iCons_eq = eq_cons_map.at(iCons);
			A_eq->insert(iCons_eq, idVar) = coeff;
		}
		else
		{
			assert(relop == ">=");
			nnz_A_ineq += 1;
			auto iCons_ineq = ineq_cons_map.at(iCons);
			A_ineq->insert(iCons_ineq, idVar) = coeff;
		}
	}

	for (auto i = 0; i < abcCallback.getC().size(); i++)
	{
		auto [idVar, coeff] = abcCallback.getC()[i];
		c->insert(idVar) = coeff;
	}
	// print out elements in A_eq
//	for (auto i = 0; i < A_eq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_eq, i); it; ++it)
//		{
//			cout << "A_eq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	// print out elements in A_ineq
//	for (auto i = 0; i < A_ineq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_ineq, i); it; ++it)
//		{
//			cout << "A_ineq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	// print out elements in b_eq
//	for (auto i = 0; i < b_eq->size(); i++)
//	{
//		cout << "b_eq(" << i << ") = " << b_eq->coeff(i) << endl;
//	}
//	// print out elements in b_ineq
//	for (auto i = 0; i < b_ineq->size(); i++)
//	{
//		cout << "b_ineq(" << i << ") = " << b_ineq->coeff(i) << endl;
//	}
//	// print out elements in c
//	for (auto i = 0; i < c->size(); i++)
//	{
//		cout << "c(" << i << ") = " << c->coeff(i) << endl;
//	}

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
