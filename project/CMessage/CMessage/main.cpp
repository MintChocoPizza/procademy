
#include <stdio.h>

#include <Windows.h>
#include "CMessage.h"

int main()
{
	CMessage* m = new CMessage;

	char* ttt = (char*)malloc(123);

	int a = 10;

	m << a;



	return 0;
}