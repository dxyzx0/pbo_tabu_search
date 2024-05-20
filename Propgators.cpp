//
// Created by psw on 5/20/24.
//

#include "Propgators.h"

#include <utility>
Propgators::Propgators(shared_ptr< Problem > prob, shared_ptr< Settings > settings)
{
	this->prob = std::move(prob);
	this->set = std::move(settings);
}
