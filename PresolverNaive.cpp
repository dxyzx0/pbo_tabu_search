//
// Created by psw on 5/20/24.
//

#include <iostream>
#include <cassert>
#include <algorithm>
#include "PresolverNaive.h"
#include "type_result.h"
#include "utils.h"

using namespace std;

PresolverNaive::PresolverNaive(shared_ptr< Problem > prob, shared_ptr< Settings > settings) : Presolver(std::move(prob),
	std::move(settings))
{
}

PresolverNaive::~PresolverNaive()
{

}

PresResult PresolverNaive::presolve()
{
	vector< long > red_cons_ineq, red_cons_eq;
	vector< long > inf_cons_ineq, inf_cons_eq;
	red_cons_ineq.reserve(prob->getNConsIneq());
	red_cons_eq.reserve(prob->getNConsEq());
	inf_cons_ineq.reserve(prob->getNConsIneq());
	inf_cons_eq.reserve(prob->getNConsEq());

	// enumerate all constraints in the problem to check if they are redundant
	// TODO: change to A_ineq->row(i).cwiseMin(0).sum() >= b_i, same for equality constraints
	auto A_ineq = prob->getA_ineq();
	auto A_ineq_r = IntSpRMat(*A_ineq);
	for (long i = 0; i < A_ineq_r.outerSize(); ++i)
	{
		IntegerType b_i = prob->getB_ineq()->coeff(i);
		if (b_i <= 0) // if b_i > 0, then the constraint is never redundant, just set x = 0 -> Ax = 0
		{
			IntegerType sum_red_row_i = 0;
			for (IntSpRMat::InnerIterator it(A_ineq_r, i); it; ++it)
			{
				sum_red_row_i += it.value() < 0 ? it.value() : 0;
			}
			if (sum_red_row_i >= b_i)
			{
				red_cons_ineq.push_back(i);
				cout_com << "Constraint " << i << " is redundant, because " << sum_red_row_i << " >= " << b_i << endl;
				A_ineq->row(i) *= 0;
			}
		}

		IntegerType sum_inf_row_i = 0;
		for (IntSpRMat::InnerIterator it(A_ineq_r, i); it; ++it)
		{
			sum_inf_row_i += it.value() > 0 ? it.value() : 0;
		}
		if (sum_inf_row_i < b_i)
		{
			inf_cons_ineq.push_back(i);
			cout_com << "Constraint " << i << " is infeasible, because " << sum_inf_row_i << " < " << b_i << endl;
			return PresResult::PRES_INFEASIBLE;
		}
	}
	auto A_eq = prob->getA_eq();
	auto A_eq_r = IntSpRMat(*A_eq);
	for (long i = 0; i < A_eq_r.outerSize(); i++)
	{
		IntegerType b_i = prob->getB_eq()->coeff(i);
		IntegerType sum_pos_row_i = 0;
		if (b_i > 0)
		{
			for (IntSpRMat::InnerIterator it(A_eq_r, i); it; ++it)
			{
				sum_pos_row_i += it.value() > 0 ? it.value() : 0;
			}
			if (sum_pos_row_i < b_i)
			{
				inf_cons_eq.push_back(i);
				cout_com << "Constraint " << i << " is infeasible, because " << sum_pos_row_i << " < " << b_i << endl;
				return PresResult::PRES_INFEASIBLE;
			}
		}
		else if (b_i < 0)
		{
			IntegerType sum_neg_row_i = 0;
			for (IntSpRMat::InnerIterator it(A_eq_r, i); it; ++it)
			{
				sum_neg_row_i += it.value() < 0 ? it.value() : 0;
			}
			if (sum_neg_row_i > b_i)
			{
				inf_cons_eq.push_back(i);
				cout_com << "Constraint " << i << " is infeasible, because " << sum_neg_row_i << " > " << b_i << endl;
				return PresResult::PRES_INFEASIBLE;
			}
		}
		else
		{
			bool red_flag = true;
			for (IntSpRMat::InnerIterator it(A_eq_r, i); it; ++it)
			{
				if (it.value() != 0)
				{
					red_flag = false;
					break;
				}
			}
			if (red_flag)
			{
				cout_com << "Constraint " << i << " is redundant" << endl;
				red_cons_eq.push_back(i);
				A_eq->row(i) *= 0;
			}
		}
	}
	// remove redundant constraints by slicing the original constraints
	// remove redundant constraints in the inequality constraints
//	long nConsIneq = A_ineq->outerSize() - red_cons_ineq.size();
//	if (nConsIneq == 0)
//	{
//		cout_com << "All inequality constraints are redundant" << endl;
//		A_ineq->setZero();
//	}
//	else
//	{
//		for (auto i : red_cons_ineq)
//		{
//			A_ineq->removeOuterVectors(i);
//			cout_com << A_ineq->rows() << A_ineq->cols() << endl;
//		}
//	}
//
//	// remove redundant constraints in the equality constraints
//	long nConsEq = A_eq->outerSize() - red_cons_eq.size();
//	if (nConsEq == 0)
//	{
//		cout_com << "All equality constraints are redundant" << endl;
//		A_eq->setZero();
//	}
//	else
//	{
//		for (auto i : red_cons_eq)
//		{
//			A_eq->removeOuterVectors(i);
//			cout_com << A_eq->rows() << A_eq->cols() << endl;
//		}
//	}

	// remove redundant constraints in the right-hand side of the inequality constraints
	auto b_ineq = prob->getB_ineq();
	for (auto iCons : red_cons_ineq)
	{
//		cout_com << (*b_ineq)[iCons] << endl;
		b_ineq->coeffRef(iCons) = 0;
//		cout_com << (*b_ineq)[iCons] << endl;
//		cout_com << b_ineq->size() << endl;
	}
	// remove redundant constraints in the right-hand side of the equality constraints
	auto b_eq = prob->getB_eq();
	for (auto iCons : red_cons_eq)
	{
		b_eq->coeffRef(iCons) = 0;
//		cout_com << b_eq->size() << endl;
	}
	assert(A_ineq->rows() == b_ineq->size());
	assert(A_eq->rows() == b_eq->size());
	prob->setNConsIneq(A_ineq->rows());
	prob->setNConsEq(A_eq->rows());
	prob->setNCons(A_ineq->rows() + A_eq->rows());

	if (!red_cons_ineq.empty() || !red_cons_eq.empty())
	{
		cout_com << "Reduced " << red_cons_ineq.size() << " inequality constraints and " << red_cons_eq.size()
				 << " equality constraints" << endl;
		return PresResult::PRES_REDUCED;
	}
	else
	{
		cout_com << "No redundant constraints found" << endl;
		return PresResult::PRES_UNCHANGED;
	}

}

void PresolverNaive::postsolve()
{

}


