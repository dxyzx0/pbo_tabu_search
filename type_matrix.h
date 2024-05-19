//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__TYPE_H_
#define PBO_HEURISTICS__TYPE_H_

// define Matrix and Vector Type
#ifdef useEigen
#include <Eigen/SparseCore>
//typedef Eigen::SparseMatrix< IntegerType, RowMajor > IntMatrix;
typedef Eigen::SparseMatrix< IntegerType > IntMatrix;
typedef Eigen::SparseVector< IntegerType > IntVector;
typedef Eigen::SparseVector< bool > BinVector;
#else
#warning this MatrixType and VectorType may not be suitable for some input file. Consider using Eigen
#error "MatrixType and VectorType must be defined"
#endif

#endif //PBO_HEURISTICS__TYPE_H_
