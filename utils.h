//
// Created by psw on 5/28/24.
//

#ifndef PBO_HEURISTICS__UTILS_H_
#define PBO_HEURISTICS__UTILS_H_

#include <memory>
#include <string>
#include "type.h"

#define FJ_LOG_PREFIX "(FJ) "
#define FJ_LOG_SOL_PREFIX "(FJSOL) "

#define PBO_LOG_COMMENT_PREFIX "c "
#define PBO_LOG_STATUS_PREFIX "s "
#define PBO_LOG_OBJ_PREFIX "o "
#define PBO_LOG_SOL_PREFIX "v "

#define cout_com std::cout << PBO_LOG_COMMENT_PREFIX << FJ_LOG_PREFIX
#define cout_stat std::cout << PBO_LOG_STATUS_PREFIX << FJ_LOG_PREFIX
#define cout_obj std::cout << PBO_LOG_OBJ_PREFIX << FJ_LOG_PREFIX
#define cout_sol std::cout << PBO_LOG_SOL_PREFIX << FJ_LOG_PREFIX

#define cout_sol_com std::cout << PBO_LOG_COMMENT_PREFIX << FJ_LOG_SOL_PREFIX

bool isFeasible(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq);

IntegerType nInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq);

IntegerType rInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq);

IntegerType normedRInf(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, const IntVec& Ax_ineq, const IntVec& Ax_eq);

std::shared_ptr< IntVec > gen_rnd_vec(long nVar, long nNonZero, std::shared_ptr< IntVec > x = nullptr, size_t seed = 0);

std::shared_ptr< IntSpVec > gen_rnd_spvec(long nVar, long nNonZero);

std::shared_ptr< IntVec > str2vec(const std::string& str);

bool test_isFeasible(const IntVec& Ax_b_ineq, const IntVec& Ax_b_eq, const IntVec& Ax_b_ineq_expected, const IntVec& Ax_b_eq_expected);

#ifdef useGMP
std::string to_string(const mpz_class& x);
#endif

#endif //PBO_HEURISTICS__UTILS_H_
