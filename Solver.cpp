//
// Created by psw on 5/20/24.
//

#include "Solver.h"

#include <utility>

Solver::Solver(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
{
	ori_prob = std::move(prob);
	set = std::move(settings);
}
