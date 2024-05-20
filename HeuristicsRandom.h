//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSRANDOM_H_
#define PBO_HEURISTICS__HEURISTICSRANDOM_H_

#include "Heuristics.h"

class HeuristicsRandom: public Heuristics
{
 public:
	explicit HeuristicsRandom( shared_ptr< Problem > prob, shared_ptr< Settings > settings = nullptr );
	HeurResult heuristic() override;

};

#endif //PBO_HEURISTICS__HEURISTICSRANDOM_H_
