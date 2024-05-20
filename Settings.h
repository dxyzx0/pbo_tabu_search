//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__SETTINGS_H_
#define PBO_HEURISTICS__SETTINGS_H_

#include <memory>
#include "type.h"

using namespace std;

class Settings
{
 public:
	// solution related
  IntegerType bestObj = numeric_limits< IntegerType >::max();
  shared_ptr< IntSpVec > bestSol = nullptr;
  // limits related
  long maxIter = 10;
};

#endif //PBO_HEURISTICS__SETTINGS_H_
