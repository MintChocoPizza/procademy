
#include <iostream>
#include <stdio.h>
#include <Windows.h>


using namespace std;


int main()
{
	int* a = (int*)malloc(101);
	int* b = (int*)malloc(sizeof(int) * 101);

	memset(a, 5, 101);

	memset(b, 6, sizeof(int) * 101);

	cout << a << endl;

	cout << a + 1 << endl;

	cout << a + 101 << endl;

	cout << a + 102 << endl;



	cout << b << endl;

	cout << b + 1 << endl;

	cout << b + 101 << endl;

	cout << b + 102 << endl;

	return 0;
}