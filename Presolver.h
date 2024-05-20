//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__PRESOLVER_H_
#define PBO_HEURISTICS__PRESOLVER_H_

#include <memory>
#include "Problem.h"
#include "Settings.h"
#include "type_result.h"

using namespace std;

// abstract class for presolver
class Presolver
{
 protected:
	shared_ptr< Problem > prob;
	shared_ptr< Settings > set;
 public:
	explicit Presolver(shared_ptr< Problem > problem, shared_ptr< Settings > settings = nullptr);
	virtual PresResult presolve() = 0;
	virtual void postsolve() = 0;
};

// Presolver 1: transform all the constraints to the form of Ax >= b
// Presolver 2: normalize the constraints by dividing the constraints by the gcd of the coefficients
// Presolver 3: simple remove redundant constraints by simply check the lower bound of Ax >= b, then remove the constraint if it is always true

#endif //PBO_HEURISTICS__PRESOLVER_H_
