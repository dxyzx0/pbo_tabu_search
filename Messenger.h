//
// Created by psw on 5/20/24.
//

#ifndef PBO_HEURISTICS__MESSENGER_H_
#define PBO_HEURISTICS__MESSENGER_H_

#include <string>
#include <iostream>
using namespace std;

class Messenger
{
 public:
	static void print(const string& str)
	{
#ifdef DEBUG
		cout << str << endl;
#endif
	}
};

#endif //PBO_HEURISTICS__MESSENGER_H_
