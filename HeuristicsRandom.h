//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSRANDOM_H_
#define PBO_HEURISTICS__HEURISTICSRANDOM_H_

#include "Heuristics.h"

class HeuristicsRandom : public Heuristics
{
 protected:
	long nTryPerRound = 10000;
 public:
	HeuristicsRandom(shared_ptr< Problem > prob, shared_ptr< Settings > settings);
	HeurResult heuristic() override;

};

#endif //PBO_HEURISTICS__HEURISTICSRANDOM_H_
