
#include <stdio.h>
#include <Windows.h>

int main()
{
	char AAA[24][81];

	printf_s("%d \n", sizeof(AAA));

	memset(AAA, 't', sizeof(AAA));


	printf_s("%d \n", AAA);

	TextOut()

	return 0;
}