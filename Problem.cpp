//
// Created by psw on 5/19/24.
//

#include <cassert>
#include <iostream>
#include <spdlog/spdlog.h>
#include "Problem.h"
#include "utils.h"

using namespace std;

Problem::Problem(PboCallback& abcCallback)
{
	cout_com << "Start to initialize Problem" << endl;
	nVar = abcCallback.getNVar();
	nCons = abcCallback.getNCons();
	nConsEq = abcCallback.getNConsEq();
	nConsIneq = abcCallback.getNConsIneq();
	nnz = abcCallback.getA().size();
	nnz_A_eq = abcCallback.getA_eq().size();
	nnz_A_ineq = abcCallback.getA_ineq().size();

	assert (nCons == abcCallback.getRelOp().size() && nCons == abcCallback.getB().size());

	spdlog::info("nConsEq = {}. nConsIneq = {}", nConsEq, nConsIneq);

	// initialize A_eq, A_ineq, b_eq, b_ineq, c
	A_eq = make_shared< IntSpMat >(nConsEq, nVar);
	A_ineq = make_shared< IntSpMat >(nConsIneq, nVar);
	b_eq = make_shared< IntVec >(nConsEq);
	b_ineq = make_shared< IntVec >(nConsIneq);
	c = make_shared< IntSpVec >(nVar);

	A_eq->setFromTriplets(abcCallback.getA_eq().begin(), abcCallback.getA_eq().end());
	A_ineq->setFromTriplets(abcCallback.getA_ineq().begin(), abcCallback.getA_ineq().end());

	for (const auto& [iCons, b] : abcCallback.getB_eq())
	{
		(*b_eq)[iCons] = b;
	}
	for (const auto& [iCons, b] : abcCallback.getB_ineq())
	{
		(*b_ineq)[iCons] = b;
	}

	spdlog::info("Initialized A_eq, A_ineq, b_eq, b_ineq");

	// update b_eq, b_ineq
//	long iCons_eq = 0;
//	long iCons_ineq = 0;
//	map< long, long > eq_cons_map, ineq_cons_map;
//	std::map< long, string > relOp_map;
//
//	size_t b_size = abcCallback.getB().size();
//	for (auto i = 0; i < b_size; i++)
//	{
//		const auto [iConsRelOp, relop] = abcCallback.getRelOp()[i];
//		const auto [iConsB, b] = abcCallback.getB()[i];
//
//		assert (iConsRelOp == iConsB);
//
//		auto iCons = iConsRelOp;
//
//		relOp_map[iCons] = relop;
//		if (relop == "=")
//		{
//			b_eq->insert(iCons_eq) = b;
//			eq_cons_map[iCons] = iCons_eq;
//			iCons_eq++;
//		}
//		else
//		{
//			assert(relop == ">=");
//			b_ineq->insert(iCons_ineq) = b;
//			ineq_cons_map[iCons] = iCons_ineq;
//			iCons_ineq++;
//		}
//	}
//
//	spdlog::info("Updated b_eq, b_ineq, create eq_cons_map, ineq_cons_map, relOp_map");

//	size_t A_size = abcCallback.getA().size();
//	for (auto i = 0; i < A_size; i++)
//	{
//		spdlog::info("i = {}", i);
//		// update stats
//		auto [iCons, idVar, coeff] = abcCallback.getA()[i];
//
//		assert (idVar >= 0 && idVar < nVar);
//		assert (iCons >= 0 && iCons < nCons);
//
//		nnz += 1;
//
//		auto relop = relOp_map.at(iCons);
//
//		spdlog::info("relop = {}", relop);
//		if (relop == "=")
//		{
//			nnz_A_eq += 1;
//			auto iCons_eq = eq_cons_map.at(iCons);
//			A_eq->insert(iCons_eq, idVar) = coeff;
//		}
//		else
//		{
//			assert(relop == ">=");
//			nnz_A_ineq += 1;
//			auto iCons_ineq = ineq_cons_map.at(iCons);
//			A_ineq->insert(iCons_ineq, idVar) = coeff;
//		}
//		spdlog::info("Finished updating A_eq, A_ineq");
//	}
//
//	spdlog::info("Updated A_eq, A_ineq");

	size_t c_size = abcCallback.getC().size();
	for (size_t i = 0; i < c_size; i++)
	{
		auto [idVar, coeff] = abcCallback.getC()[i];
		c->insert(idVar) = coeff;
	}

	spdlog::info("Updated c");
	// print out elements in A_eq
//	for (auto i = 0; i < A_eq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_eq, i); it; ++it)
//		{
//			cout_com << "A_eq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	// print out elements in A_ineq
//	for (auto i = 0; i < A_ineq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_ineq, i); it; ++it)
//		{
//			cout_com << "A_ineq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	// print out elements in b_eq
//	for (auto i = 0; i < b_eq->size(); i++)
//	{
//		cout_com << "b_eq(" << i << ") = " << b_eq->coeff(i) << endl;
//	}
//	// print out elements in b_ineq
//	for (auto i = 0; i < b_ineq->size(); i++)
//	{
//		cout_com << "b_ineq(" << i << ") = " << b_ineq->coeff(i) << endl;
//	}
//	// print out elements in c
//	for (auto i = 0; i < c->size(); i++)
//	{
//		cout_com << "c(" << i << ") = " << c->coeff(i) << endl;
//	}

	assert(nConsEq + nConsIneq == nCons);
	assert(nnz_A_eq + nnz_A_ineq == nnz);

	A_ineq->finalize();
	A_eq->finalize();
	c->finalize();

	// summarize the problem
	cout_com << "nVar = " << nVar << endl;
	cout_com << "nCons = " << nCons << " (nConsEq = " << nConsEq << ", nConsIneq = " << nConsIneq << ")" << endl;
	cout_com << "nnz = " << nnz << " (nnz_A_eq = " << nnz_A_eq << ", nnz_A_ineq = " << nnz_A_ineq << ")" << endl;
	cout_com << "nnz_c = " << c->nonZeros() << endl;

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
	b_eq = make_shared< IntVec >(*prob.b_eq);
	b_ineq = make_shared< IntVec >(*prob.b_ineq);
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
	b_eq = make_shared< IntVec >(*prob.b_eq);
	b_ineq = make_shared< IntVec >(*prob.b_ineq);
	c = make_shared< IntSpVec >(*prob.c);

	return *this;
}

bool Problem::isFeasible(const IntSpVec& x)
{
	// check if x is feasible
	// check if A_eq * x = b_eq
//	cout_com << "x = " << x << endl;
//	for (auto i = 0; i < x.size(); i++)
//	{
//		cout_com << "x(" << i << ") = " << x.coeff(i) << endl;
//	}
//	cout_com << "A_eq = " << *A_eq << endl;
//	for (auto i = 0; i < A_eq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_eq, i); it; ++it)
//		{
//			cout_com << "A_eq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	cout_com << "b_eq = " << *b_eq << endl;
//	for (auto i = 0; i < b_eq->size(); i++)
//	{
//		cout_com << "b_eq(" << i << ") = " << b_eq->coeff(i) << endl;
//	}
//	cout_com << "A_ineq = " << *A_ineq << endl;
//	for (auto i = 0; i < A_ineq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_ineq, i); it; ++it)
//		{
//			cout_com << "A_ineq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	cout_com << "b_ineq = " << *b_ineq << endl;
//	for (auto i = 0; i < b_ineq->size(); i++)
//	{
//		cout_com << "b_ineq(" << i << ") = " << b_ineq->coeff(i) << endl;
//	}

	auto lhs_eq = (*A_eq) * x;
	auto lhs_ineq = (*A_ineq) * x;
//	cout_com << "lhs_eq = " << lhs_eq << endl;
//	cout_com << "lhs_ineq = " << lhs_ineq << endl;

	auto b_eq_sp = b_eq->sparseView();
	auto b_ineq_sp = b_ineq->sparseView();
	auto res_eq = lhs_eq.cwiseEqual(b_eq_sp).cast< long >();
	auto res_ineq = lhs_ineq.cwiseMax(b_ineq_sp).cwiseEqual(lhs_ineq).cast< long >();
//	cout_com << "res_eq = " << res_eq << endl;
//	cout_com << "res_ineq = " << res_ineq << endl;
//	cout_com << "res_eq.sum() = " << res_eq.sum() << endl;
//	cout_com << "nConsEq = " << nConsEq << endl;
//	cout_com << "res_ineq.sum() = " << res_ineq.sum() << endl;
//	cout_com << "nConsIneq = " << nConsIneq << endl;

	if (res_eq.sum() == nConsEq && res_ineq.sum() == nConsIneq)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Problem::isFeasible(const IntVec& x)
{
	// check if x is feasible
	// check if A_eq * x = b_eq
//	cout_com << "x = " << x << endl;
//	for (auto i = 0; i < x.size(); i++)
//	{
//		cout_com << "x(" << i << ") = " << x.coeff(i) << endl;
//	}
//	cout_com << "A_eq = " << *A_eq << endl;
//	for (auto i = 0; i < A_eq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_eq, i); it; ++it)
//		{
//			cout_com << "A_eq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	cout_com << "b_eq = " << *b_eq << endl;
//	for (auto i = 0; i < b_eq->size(); i++)
//	{
//		cout_com << "b_eq(" << i << ") = " << b_eq->coeff(i) << endl;
//	}
//	cout_com << "A_ineq = " << *A_ineq << endl;
//	for (auto i = 0; i < A_ineq->outerSize(); i++)
//	{
//		for (IntSpMat::InnerIterator it(*A_ineq, i); it; ++it)
//		{
//			cout_com << "A_ineq(" << i << ", " << it.index() << ") = " << it.value() << endl;
//		}
//	}
//	cout_com << "b_ineq = " << *b_ineq << endl;
//	for (auto i = 0; i < b_ineq->size(); i++)
//	{
//		cout_com << "b_ineq(" << i << ") = " << b_ineq->coeff(i) << endl;
//	}

	auto lhs_eq = (*A_eq) * x;
	auto lhs_ineq = (*A_ineq) * x;
//	cout_com << "lhs_eq = " << lhs_eq << endl;
//	cout_com << "lhs_ineq = " << lhs_ineq << endl;

	auto res_eq = lhs_eq.cwiseEqual(*b_eq).cast< long >();
	auto res_ineq = lhs_ineq.cwiseMax(*b_ineq).cwiseEqual(lhs_ineq).cast< long >();
//	cout_com << "res_eq = " << res_eq << endl;
//	cout_com << "res_ineq = " << res_ineq << endl;
//	cout_com << "res_eq.sum() = " << res_eq.sum() << endl;
//	cout_com << "nConsEq = " << nConsEq << endl;
//	cout_com << "res_ineq.sum() = " << res_ineq.sum() << endl;
//	cout_com << "nConsIneq = " << nConsIneq << endl;

	if (res_eq.sum() == nConsEq && res_ineq.sum() == nConsIneq)
	{
		return true;
	}
	else
	{
		return false;
	}
}

IntegerType Problem::calNInf(const IntVec& x)
{
	auto lhs_eq = (*A_eq) * x;
	auto lhs_ineq = (*A_ineq) * x;
//	cout_com << "lhs_eq = " << lhs_eq << endl;
//	cout_com << "lhs_ineq = " << lhs_ineq << endl;

	auto res_eq = lhs_eq.cwiseEqual(*b_eq).cast< long >();
	auto res_ineq = lhs_ineq.cwiseMax(*b_ineq).cwiseEqual(lhs_ineq).cast< long >();
	return res_eq.sum() + res_ineq.sum();
}

IntegerType Problem::getObj(const IntSpVec& x)
{
	return c->dot(x);
}

IntegerType Problem::getObj(const IntVec& x)
{
	return c->dot(x);
}

