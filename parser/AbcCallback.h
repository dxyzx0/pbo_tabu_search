//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS__ABCCALLBACK_H_
#define PBO_HEURISTICS__ABCCALLBACK_H_

#include <vector>
#include <memory>
#include <string>
#include "DefaultCallback.h"
#include "../type_int.h"

using namespace std;

class AbcCallback : public DefaultCallback
{
 private:
	vector< tuple< long, long, IntegerType >> A;
	vector< tuple< long, string>> relOp;
	vector< tuple< long, IntegerType>> b;
	vector< tuple< long, IntegerType>> c;
	long nVar;
	long nCons;
	long iCons;
 public:
	void metaData(int nbvar, int nbconstr) override;
//    void beginObjective() override;
//    void endObjective() override;
	void objectiveTerm(IntegerType coeff, int idVar) override;
//    void beginConstraint() override;
	void endConstraint() override;
	void constraintTerm(IntegerType coeff, int idVar) override;
	void constraintRelOp(std::string relop) override;
	void constraintRightTerm(IntegerType val) override;
	void linearizeProduct(int newSymbol, std::vector< int > product) override;
	std::vector< std::tuple< long, long, IntegerType >> getA();
	std::vector< std::tuple< long, string>> getRelOp();
	std::vector< std::tuple< long, IntegerType>> getB();
	std::vector< std::tuple< long, IntegerType>> getC();
	long getNVar() const
	{
		return nVar;
	}
	long getNCons() const
	{
		return nCons;
	}

};

#endif //PBO_HEURISTICS__ABCCALLBACK_H_
