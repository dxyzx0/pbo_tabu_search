//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__TYPE_H_
#define PBO_HEURISTICS__TYPE_H_

// define IntegerType
#ifdef useGMP
#include <gmpxx.h>

typedef
mpz_class IntegerType;
#else
#warning this IntegerType may not be suitable for some input file. Consider using GMP
typedef
long IntegerType;
#endif

#endif //PBO_HEURISTICS__TYPE_H_
