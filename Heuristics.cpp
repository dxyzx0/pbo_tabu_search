//
// Created by psw on 5/20/24.
//

#include "Heuristics.h"
Heuristics::Heuristics(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
{
	this->prob = std::move(prob);
	this->set = std::move(settings);
}
