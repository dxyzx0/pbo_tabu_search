//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__SETTINGS_H_
#define PBO_HEURISTICS__SETTINGS_H_

#include <memory>
#include <chrono>
#include "type.h"
#include "Messenger.h"

using namespace std;

class Settings
{
 public:
	// constructor
	Settings() = default;

	// solution related
	IntegerType bestObj = PBOINTMAX;
	shared_ptr< IntVec > bestSol = nullptr;

	std::chrono::steady_clock::time_point startTime;
	double timelimit = 3600;
	int numthreads = 1;
	int verbose = 0;

	// limits related
	long maxIter = 10;
	long solLimit = 1;

	// output related
	shared_ptr< Messenger > msg = make_shared< Messenger >();
};

#endif //PBO_HEURISTICS__SETTINGS_H_
