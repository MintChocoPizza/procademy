
#include <iostream>
#include <stdio.h>

using namespace std;

class AAA;

struct st_temp
{
	AAA* ghighi;
};


class AAA
{
public:
	int a;
};



int main()
{

	st_temp temp;
	temp.ghighi->a;

	return 0;
}

