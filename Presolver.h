//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__PRESOLVER_H_
#define PBO_HEURISTICS__PRESOLVER_H_

class Presolver {
  public:
    virtual void presolve() = 0;
    virtual void postsolve() = 0;
};

// Presolver 1: transform all the constraints to the form of Ax >= b
// Presolver 2: normalize the constraints by dividing the constraints by the gcd of the coefficients
// Presolver 3: simple remove redundant constraints by simply check the lower bound of Ax >= b, then remove the constraint if it is always true

#endif //PBO_HEURISTICS__PRESOLVER_H_
