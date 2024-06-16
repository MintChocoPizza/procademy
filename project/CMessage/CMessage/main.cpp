
#include <stdio.h>

#include <Windows.h>
#include "CMessage.h"

int main()
{
	SerializeBuffer sb;

	char ttt[] = "asdfadsf";

	sb.PutData(ttt, sizeof(ttt));



	return 0;
}