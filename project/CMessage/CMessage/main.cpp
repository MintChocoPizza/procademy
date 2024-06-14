
#include <stdio.h>

#include <Windows.h>
#include "CMessage.h"

int main()
{
	CMessage* m = new CMessage;

	char* ttt = (char*)malloc(123);

	m->PutData(ttt, 123);

	printf_s("Hello World \n");

	int size = 12;

	if (m->countFreeSize(12))
	{
		m << 12;
	}

	return 0;
}