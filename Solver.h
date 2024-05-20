//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__SOLVER_H_
#define PBO_HEURISTICS__SOLVER_H_

#include <memory>
#include <vector>
#include "Problem.h"
#include "Presolver.h"
#include "Propgators.h"
#include "Heuristics.h"
#include "Settings.h"
#include "type_result.h"

using namespace std;

class Solver
{
 private:
	shared_ptr< Problem > ori_prob;
	shared_ptr< Problem > tran_prob;
	vector< shared_ptr< Presolver > > presolvers;
	vector< shared_ptr< Propgators > > propgators;
	vector< shared_ptr< Heuristics > > heuristics;
	shared_ptr< Settings > set;
 public:
	explicit Solver(shared_ptr< Problem > prob, shared_ptr< Settings > settings = nullptr);
	void addDefaultPlugins();
	SolveResult solve();
};

#endif //PBO_HEURISTICS__SOLVER_H_
