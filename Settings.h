//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__SETTINGS_H_
#define PBO_HEURISTICS__SETTINGS_H_

#include <memory>
#include "type.h"
#include "Messenger.h"

using namespace std;

class Settings
{
 public:
	// constructor
	Settings() = default;

	// solution related
	IntegerType bestObj = 2e50;  // FIXME: numeric_limits< IntegerType >::max() has bug
	shared_ptr< IntSpVec > bestSol = nullptr;

	// limits related
	long maxIter = 10;

	// output related
	shared_ptr< Messenger > msg = make_shared< Messenger >();
};

#endif //PBO_HEURISTICS__SETTINGS_H_
