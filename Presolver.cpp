//
// Created by psw on 5/19/24.
//

#include "Presolver.h"
Presolver::Presolver(shared_ptr< Problem > problem, shared_ptr< Settings > settings)
{
	prob = std::move(problem);
	set = std::move(settings);
}
