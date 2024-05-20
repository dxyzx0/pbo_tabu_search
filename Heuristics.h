//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__HEURISTICS_H_
#define PBO_HEURISTICS__HEURISTICS_H_

#include <memory>
#include "Problem.h"
#include "Settings.h"
#include "type_result.h"

class Heuristics
{
 protected:
	shared_ptr< Problem > prob;
	shared_ptr< Settings > set;
 public:
	explicit Heuristics(shared_ptr< Problem > prob, shared_ptr< Settings > settings = nullptr);
	virtual HeurResult heuristic() = 0;

};

#endif //PBO_HEURISTICS__HEURISTICS_H_
