//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__TYPE_INT_H_
#define PBO_HEURISTICS__TYPE_INT_H_

// define IntegerType
#ifdef useGMP
#include <gmpxx.h>

typedef mpz_class IntegerType;

const IntegerType PBOINTMAX = 2e60;
const IntegerType PBOINTMIN = -PBOINTMAX; // FIXME: numeric_limits< mpz_class >::min() has bug

#else
//#warning this IntegerType may not be suitable for some input file. Consider using GMP
#include <limits>
typedef long IntegerType;

const IntegerType PBOINTMAX = std::numeric_limits< IntegerType >::max();
const IntegerType PBOINTMIN = std::numeric_limits< IntegerType >::min();

#endif

#endif //PBO_HEURISTICS__TYPE_INT_H_
