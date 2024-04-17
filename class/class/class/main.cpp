
#include <iostream>

using namespace std;

struct st_NODE
{
	st_NODE* Parents;
	st_NODE* LC;
	st_NODE* RC;

	int Data;
};

class CTEST
{
public:
	CTEST()
	{
		a = 10;

		Root = (st_NODE*)malloc(sizeof(st_NODE));

		Root->Data = 0;

		Root->LC = (st_NODE*)malloc(sizeof(st_NODE));
		Root->LC->Data = 1;

		Root->RC = (st_NODE*)malloc(sizeof(st_NODE));
		Root->RC->Data = 2;

		Root->RC->RC = (st_NODE*)malloc(sizeof(st_NODE));
		Root->RC->RC->Data= 3;

		
		cout << "생성자 입니다." << endl;
	}
	~CTEST()
	{
		a = 0;
		cout << "소멸자 입니다. " << endl;
	}

private:
	int a;
public: st_NODE* Root;
};

int main()
{
	//CTEST* cTest = new CTEST;
	//CTEST* a = cTest;
	//CTEST*& b = cTest;


	//st_NODE* LC = cTest->Root->LC;
	//st_NODE* RRC = cTest->Root->RC->RC;

	//// 포인터 

	//LC->Data = 0xff; 
	//cTest->Root->LC->Data = 0xee;

	//cout << endl;

	//RRC->Data = 0xff;
	//cTest->Root->RC->RC->Data = 0xee;

	//cout << RRC->Data << cTest->Root->RC->RC->Data;

	//cout << endl;
	

	int a;
	int b;
	int c;

	a = 1;
	b = 2;
	c = a + b;
	c = a * 3;



	return 0;
}