
#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include <list>

#include "CList.h"

using namespace std;

class A
{
public:
	int a;
};

int main()
{
	CList<A> c_li;

	A a;
	a.a = 10;

	c_li.push_back(a);

	CList<A>::iterator T;

	T = c_li.begin();

	cout << T._node->_data.a << endl;

	return 0;
}