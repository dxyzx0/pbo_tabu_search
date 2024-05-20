//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__PROPGATORS_H_
#define PBO_HEURISTICS__PROPGATORS_H_

#include <memory>
#include "Problem.h"
#include "Settings.h"
#include "type_result.h"

class Propgators
{
 protected:
	shared_ptr< Problem > prob;
	shared_ptr< Settings > set;
 public:
	Propgators(shared_ptr< Problem > prob, shared_ptr< Settings > settings);
	virtual PropResult propagate() = 0;

};

#endif //PBO_HEURISTICS__PROPGATORS_H_
