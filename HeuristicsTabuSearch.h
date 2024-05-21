//
// Created by psw on 5/21/24.
//

#ifndef PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
#define PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_

#include "HeuristicsRandom.h"

class HeuristicsTabuSearch: public HeuristicsRandom
{
 public:
  HeurResult heuristic() override;

};

#endif //PBO_HEURISTICS__HEURISTICSTABUSEARCH_H_
