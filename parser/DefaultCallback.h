//
// Created by psw on 5/19/24.
//

#ifndef PBO_HEURISTICS_PARSER_DEFAULTCALLBACK_H_
#define PBO_HEURISTICS_PARSER_DEFAULTCALLBACK_H_

#include <vector>
#include "../type_int.h"

using std::vector;

class DefaultCallback {
 public:
  void metaData(int nbvar, int nbconstr);
  void beginObjective();
  void endObjective();
  void objectiveTerm(IntegerType coeff, int idVar);
  void objectiveProduct(IntegerType coeff, std::vector<int> list);
  void beginConstraint();
  void endConstraint();
  void constraintTerm(IntegerType coeff, int idVar);
  void constraintProduct(IntegerType coeff, std::vector<int> list);
  void constraintRelOp(std::string relop);
  void constraintRightTerm(IntegerType val);
  void linearizeProduct(int newSymbol, std::vector<int> product);
};

#endif //PBO_HEURISTICS_PARSER_DEFAULTCALLBACK_H_
