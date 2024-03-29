#include <stdio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

/*
int main()
{
	errno_t err;
	FILE* pFile;
	long lFileSize;

	unsigned char* Buff;
	unsigned char* temp;

	err = fopen_s(&pFile, "test.txt", "rb, ccs=UNICODE");
	
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
*/

class aaa
{
public:
	aaa(int t) : a(t), b(t)
	{
		cout << "aaa 持失切" << endl;
	}
	~aaa()
	{
		cout << "aaa 社瑚切" << endl;
	}

private:
	int a;


protected:
	int b;
};

class bbb : public aaa
{
public:
	bbb(int t) : aaa(t)
	{
		b = 10;
		cout << "bbb 持失切 " << endl;
	}
	~bbb()
	{
		cout << "bbb 社瑚切" << endl;
	}

	void ttt()
	{
		cout << b << endl;
	}

private:

};


int main()
{
	bbb bb(1);

	bb.ttt();

	return 0;
}


