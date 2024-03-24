#include <stdio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	errno_t err;
	FILE* pFile;
	long lFileSize;

	unsigned char* Buff;
	unsigned char* temp;

	err = fopen_s(&pFile, "test.txt", "r, ccs=UNICODE");
	
	fseek(pFile, 0, SEEK_END);
	lFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	Buff = (unsigned char*)malloc(lFileSize);
	temp = Buff + lFileSize;

	cout << lFileSize << endl;

	fread_s(Buff, lFileSize, 1, lFileSize, pFile);
	fclose(pFile);


	

	return 0;
}