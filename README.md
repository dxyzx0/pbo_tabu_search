# Tabu Search Algorithm for the Pseudo-Boolean Optimization Problem

## Introduction
This repository contains a C++ implementation of the Tabu Search algorithm for the Pseudo-Boolean Optimization Problem.
It contains the support for big integers (> 64 bits) using the `gmp` library.

## Requirements
- Eigen 3.4.1 or higher. You can also clone the repository from [gitlab](https://gitlab.com/libeigen/eigen), move `Eigen` folder to the root dir, and change some `#include` in the source files. I believe you only need to change the `#include` in the `parser` directory.
- (Optional) GMP library if you want to solve problems with big integers (> 64 bits).