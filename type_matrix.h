//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__TYPE_MATRIX_H_
#define PBO_HEURISTICS__TYPE_MATRIX_H_

// define Matrix and Vector Type
#ifdef useEigen
#include "Eigen/Sparse"
typedef Eigen::SparseMatrix< IntegerType, Eigen::ColMajor > IntSpMat;
typedef Eigen::SparseMatrix< IntegerType, Eigen::RowMajor > IntSpRMat;
typedef Eigen::SparseVector< IntegerType > IntSpVec;
typedef Eigen::SparseVector< bool > BinSpVec;
#include "Eigen/Dense"
typedef Eigen::Matrix< IntegerType, Eigen::Dynamic, Eigen::Dynamic > IntMat;
typedef Eigen::Vector< IntegerType, Eigen::Dynamic > IntVec;
#else
#warning this MatrixType and VectorType may not be suitable for some input file. Consider using Eigen
#error "MatrixType and VectorType must be defined"
#endif

#endif //PBO_HEURISTICS__TYPE_H_
