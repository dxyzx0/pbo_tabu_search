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
 * s.t. Ax >= b
 *     x \in {0, 1}^n
 */
class Problem {
 private:
    IntMatrix A;
    IntVector b;
    IntVector c;
    long nVar;
    long nRow;

};

#endif //PBO_HEURISTICS__PROBLEM_H_
