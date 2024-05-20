//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__PRESOLVERNAIVE_H_
#define PBO_HEURISTICS__PRESOLVERNAIVE_H_

#include "Presolver.h"
#include "type.h"
class PresolverNaive: public Presolver
{
 private:
	vector< IntSpVec > A_eq_red;
	vector< IntSpVec > A_ineq_red;
public:
	explicit PresolverNaive( shared_ptr< Problem > prob, shared_ptr< Settings > settings = nullptr );
	~PresolverNaive();
	PresResult presolve() override;
	void postsolve() override;
};

#endif //PBO_HEURISTICS__PRESOLVERNAIVE_H_
