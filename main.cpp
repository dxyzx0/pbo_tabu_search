//
// Created by psw on 5/19/24.
//

#include <iostream>
#include "type.h"

int main() {
  long nVar = 10;
  long nRow = 5;
  IntMatrix A( nRow, nVar );
  IntVector b( nRow );
  IntVector x( nVar );

  A.insert( 0, 0 ) = 1;
  A.insert( 1, 1 ) = 1;
  x.insert( 0 ) = 1;
  x.insert( 1 ) = 2;
  std::cout << A * x << std::endl;
  return 0;
}