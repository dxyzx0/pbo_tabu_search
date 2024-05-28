//
// Created by psw on 5/28/24.
//

#ifndef PBO_HEURISTICS__UTILS_H_
#define PBO_HEURISTICS__UTILS_H_

#include <memory>
#include "type.h"

bool isFeasible(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq);

IntegerType nInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq);

IntegerType rInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq);

IntegerType normedRInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, const IntVec& Ax_ineq, const IntVec& Ax_eq);

std::shared_ptr< IntVec > gen_rnd_vec(long nVar, long nNonZero, std::shared_ptr< IntVec > x = nullptr);

std::shared_ptr< IntSpVec > gen_rnd_spvec(long nVar, long nNonZero);

std::shared_ptr< IntVec > str2vec(const std::string& str);

bool test_isFeasible(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, const IntVec& Ax_b_ineq_expected, const IntVec& Ax_b_eq_expected);

#endif //PBO_HEURISTICS__UTILS_H_
