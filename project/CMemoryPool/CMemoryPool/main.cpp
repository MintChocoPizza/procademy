// CMemoryPool.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "CMemoryPool.h"

using namespace std;
using namespace OreoPizza;


int num = 0;

class CTest
{
public:
	CTest()
	{
		a = 10;
		cout << ++num << " CTest 의 생성자 입니다." << endl;
	}
	~CTest()
	{
		a = 0;
		cout << num-- << "CTest의 소멸자 입니다. " << endl;
	}

private:
	int a;
};

int main()
{
	CMemoryPool<CTest> CMemoryPool(10, true);
	CMemoryPool.TraverseMemoryPool();
	cout << "여기까지 기존의 메모리 풀 입니다. " << endl << endl;

	CTest* pData1 = CMemoryPool.Alloc();
	CTest* pData2 = CMemoryPool.Alloc();
	CTest* pData3 = CMemoryPool.Alloc();
	CTest* pData4 = CMemoryPool.Alloc();
	CMemoryPool.TraverseMemoryPool();
	cout << "여기까지 할당했을 때 메모리 풀 입니다. " << endl << endl;


	CMemoryPool.Free(pData1);
	CMemoryPool.Free(pData2);
	CMemoryPool.Free(pData3);
	CMemoryPool.Free(pData4);
	CMemoryPool.TraverseMemoryPool();
	cout << "여기까지 메모리 해지했을 때 메모리 풀 입니다. " << endl << endl;
	

	cout << "Hello World" << endl;

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
