// CMemoryPool.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include <vector>

#include "MyProfile.h"
#include "CMemoryPool.h"

using namespace std;
using namespace OreoPizza;

class CTEST
{
	// 64번씩 1만번
public:
	CTEST();
	~CTEST();
private:
	double a;
	int b;
	short c;
	char d;
};

CTEST::CTEST() : a(1), b(2), c(3), d(4)
{
	
}

CTEST::~CTEST() 
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
}


int main()
{
	// 1024byte 1만번 기준	

	CTEST* Arr[64];
	int temp;

	ProfileReset();

	CMemoryPool<CTEST> MemPool(0, FALSE);
	{
		for (int iCnt = 0; iCnt < 100; iCnt++)
		{
			cMYPROFILE cTest(_T("MemPool"));
			for (int i = 0; i < 10000; ++i)
			{
				for (int j = 0; j < 64; ++j)
				{
					Arr[j] = MemPool.Alloc();
				}
				for (int j = 0; j < 64; ++j)
				{
					MemPool.Free(Arr[j]);
				}
			}
		}
	}

	{
		for (int iCnt = 0; iCnt < 100; ++iCnt)
		{
			cMYPROFILE cTest(_T("New"));

			for (int i = 0; i < 10000; ++i)
			{
				for (int j = 0; j < 64; ++j)
				{
					Arr[j] = new CTEST();
				}

				for (int j = 0; j < 64; ++j)
				{
					delete Arr[j];
				}
			}
		}
	}
	// 약 8배 정도 빠르다.


	ProfileDataOutText(L"AAA.txt");
	return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
