//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__PROBLEM_H_
#define PBO_HEURISTICS__PROBLEM_H_

#include "type.h"

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
class Problem {
 private:
  IntSpMat A_eq;
  IntSpMat A_ineq;
  IntSpVec b_eq;
  IntSpVec b_ineq;
  IntSpVec c;
  long nVar;
  long nCons;
  long nConsEq;
  long nConsIneq;

};

#endif //PBO_HEURISTICS__PROBLEM_H_
