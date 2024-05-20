//
// Created by psw on 5/19/24.
//

#include "AbcCallback.h"

#include <iostream>

using namespace std;

void AbcCallback::objectiveTerm(IntegerType coeff, int idVar)
{
	DefaultCallback::objectiveTerm(coeff, idVar);
	c.emplace_back(idVar - 1, coeff);
}

void AbcCallback::metaData(int nbvar, int nbconstr)
{
	DefaultCallback::metaData(nbvar, nbconstr);
	nVar = nbvar;
	nCons = nbconstr;
}

void AbcCallback::constraintTerm(IntegerType coeff, int idVar)
{
	DefaultCallback::constraintTerm(coeff, idVar);
	A.emplace_back(iCons, idVar - 1, coeff);  // idVar is 1-based, so we need to subtract 1
}

void AbcCallback::endConstraint()
{
	DefaultCallback::endConstraint();
	iCons += 1;
}

void AbcCallback::constraintRelOp(std::string relop)
{
	DefaultCallback::constraintRelOp(relop);
	relOp.emplace_back(iCons, relop);
}

void AbcCallback::constraintRightTerm(IntegerType val)
{
	DefaultCallback::constraintRightTerm(val);
	b.emplace_back(iCons, val);
}

void AbcCallback::linearizeProduct(int newSymbol, std::vector< int > product)
{
	DefaultCallback::linearizeProduct(newSymbol, product);
	throw std::runtime_error("Not implemented");
}

std::vector< std::tuple< long, long, IntegerType > > AbcCallback::getA()
{
	return A;
}

std::vector< std::tuple< long, string > > AbcCallback::getRelOp()
{
	return relOp;
}

std::vector< std::tuple< long, IntegerType > > AbcCallback::getB()
{
	return b;
}
std::vector< std::tuple< long, IntegerType > > AbcCallback::getC()
{
	return c;
}
