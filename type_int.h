//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__TYPE_INT_H_
#define PBO_HEURISTICS__TYPE_INT_H_

// define IntegerType
#ifdef useGMP
#include <gmpxx.h>

typedef mpz_class IntegerType;

#define PBOINTMAX 2e50 // FIXME: numeric_limits< mpz_class >::max() has bug
#define PBOINTMIN -2e50 // FIXME: numeric_limits< mpz_class >::min() has bug

#else

#warning this IntegerType may not be suitable for some input file. Consider using GMP
typedef long IntegerType;

#define PBOINTMAX numeric_limits< IntegerType >::max()
#define PBOINTMIN numeric_limits< IntegerType >::min()

#endif

#endif //PBO_HEURISTICS__TYPE_INT_H_
