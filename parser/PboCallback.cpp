//
// Created by psw on 5/19/24.
//

#include <iostream>
#include <cassert>
#include "PboCallback.h"

using namespace std;

void PboCallback::objectiveTerm(IntegerType coeff, int idVar)
{
	DefaultCallback::objectiveTerm(coeff, idVar);
	c.emplace_back(idVar - 1, coeff);
}

void PboCallback::metaData(int nbvar, int nbconstr)
{
	DefaultCallback::metaData(nbvar, nbconstr);
	nVar = nbvar;
	nCons = nbconstr;
}

void PboCallback::constraintTerm(IntegerType coeff, int idVar)
{
	DefaultCallback::constraintTerm(coeff, idVar);
	A.emplace_back(iCons, idVar - 1, coeff);  // idVar is 1-based, so we need to subtract 1
	tmp_trip.emplace_back(idVar - 1, coeff);
}

void PboCallback::endConstraint()
{
	DefaultCallback::endConstraint();

	vector< Eigen::Triplet< IntegerType > > tmp_triplet;

	if (iRelOp == "=")
	{
		for (auto& t : tmp_trip)
		{
			tmp_triplet.emplace_back(iConsEq, get<0>(t), get<1>(t));
		}
		A_eq_trip.insert(A_eq_trip.end(), tmp_triplet.begin(), tmp_triplet.end());
		b_eq.emplace_back(iConsEq, get<1>(b.back()));
		iConsEq++;
	}
	else
	{
		assert(iRelOp == ">=");  // only support >= for now
		for (auto& t : tmp_trip)
		{
			tmp_triplet.emplace_back(iConsIneq, get<0>(t), get<1>(t));
		}
		A_ineq_trip.insert(A_ineq_trip.end(), tmp_triplet.begin(), tmp_triplet.end());
		b_ineq.emplace_back(iConsIneq, get<1>(b.back()));
		iConsIneq++;
	}

//	for (auto& t : tmp_triplet)
//	{
//		cout << t.row() << " " << t.col() << " " << t.value() << endl;
//	}
//	cout << "RelOp: " << iRelOp << endl;
//	cout << "b: " << get<1>(b.back()) << endl;

	tmp_trip.clear();
	iCons ++;

}

void PboCallback::constraintRelOp(std::string relop)
{
	DefaultCallback::constraintRelOp(relop);
	relOp.emplace_back(iCons, relop);
	iRelOp = relop;
}

void PboCallback::constraintRightTerm(IntegerType val)
{
	DefaultCallback::constraintRightTerm(val);
	b.emplace_back(iCons, val);
}

void PboCallback::linearizeProduct(int newSymbol, std::vector< int > product)
{
	DefaultCallback::linearizeProduct(newSymbol, product);
	throw std::runtime_error("Not implemented");
}

const std::vector< std::tuple< long, long, IntegerType > >& PboCallback::getA()
{
	return A;
}

const std::vector< Eigen::Triplet < IntegerType > >& PboCallback::getA_eq()
{
	return A_eq_trip;
}

const std::vector< Eigen::Triplet < IntegerType > >& PboCallback::getA_ineq()
{
	return A_ineq_trip;
}


const std::vector< std::tuple< long, string > >& PboCallback::getRelOp()
{
	return relOp;
}

const std::vector< std::tuple< long, IntegerType > >& PboCallback::getB()
{
	return b;
}

const std::vector< std::tuple< long, IntegerType > >& PboCallback::getB_eq()
{
	return b_eq;
}

const std::vector< std::tuple< long, IntegerType > >& PboCallback::getB_ineq()
{
	return b_ineq;
}

const std::vector< std::tuple< long, IntegerType > >& PboCallback::getC()
{
	return c;
}
