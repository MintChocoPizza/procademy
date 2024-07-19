// Ring_Buffer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//



#include <Windows.h>
#include <process.h>
#include <iostream>

#include "C_Ring_Buffer.h"
#include "OSJ_Test.h"


#define VERSION 0

using namespace std;

constexpr int CONSOLE_WIDTH = 237;
char testString[] = "@234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*!";
size_t len;
C_RING_BUFFER g_rb;


std::queue<char> Q;
char TempBuf[10000 + 2];



unsigned _stdcall QueuePush(void* pArg);
unsigned _stdcall QueuePop(void* pArg);


int main()
{
#if VERSION == 0
	HANDLE hThread[2];
	DWORD dwStatus;
	
	len = strlen(testString);

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, QueuePush, NULL, NULL, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, QueuePop, NULL, NULL, NULL);

	dwStatus = WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	if (dwStatus == WAIT_OBJECT_0)
	{
		printf("ALL Thread normal shutdown !!!!! \n");
	}
	else
	{
		wprintf(L"WaitForMultipleObject %d error \n", GetLastError());

	}
#elif VERSION == 1
	OSJ_test();
#elif VERSION == 2
	len = strlen(testString);
	while (1)
	{
		QueuePush(NULL);
		QueuePop(NULL);
	}
#endif

    std::cout << "Hello World!\n";
}

unsigned _stdcall QueuePush(void* pArg)
{

	int RandomByteToExtract;
	int temp;
	int index;
	size_t rbFreeSize;
	size_t qSize;
	size_t EnqueueSize;

	srand((unsigned int)GetThreadId(NULL));

#if VERSION == 0
	while (true)
#elif VERSION == 2
#endif
	{
		memset(TempBuf, 0, sizeof(TempBuf));

		// 큐 size가 너무 크다면 한동안 Enqueue만 한다.
		if (Q.size() <= 100000)
		{
			// 큐에 문자열을 저장한다. 
			for (int i = 0; i < len; ++i)
			{
				Q.push(testString[i]);
			}
		}

		// 현재 Queue의 사이즈
		qSize = Q.size();

		// 현재 링버퍼 FreeSize
		rbFreeSize = g_rb.GetFreeSize();

		// 현재 링버퍼의 FreeSize가 0이라면 이번 루프 건너 뛴다.
		if (rbFreeSize == 0)
		{
#if VERSION == 0
			continue;
#elif VERSION == 2
			return 0;
#endif
		}

		// 큐 사이즈와, 링버퍼 FreeSize 중 더 작은 값을 기준으로 랜덤한 값을 생성한다.
		RandomByteToExtract = rand() % (qSize > rbFreeSize ? rbFreeSize : qSize) + 1;

		// 큐에서 원하는 바이트 수 만큼 추출하여 임의의 버퍼에 집어넣는다.
		index = 0;
		temp = RandomByteToExtract;
		while (temp > 0 && !Q.empty())
		{
			TempBuf[index++] = Q.front();
			Q.pop();
			temp--;
		}

		EnqueueSize = g_rb.Enqueue(TempBuf, RandomByteToExtract);
	}

	return 0;
}

unsigned _stdcall QueuePop(void* pArg)
{
	char dequeueBuf[CONSOLE_WIDTH + 2];
	int RandomByteToExtract;
	size_t rbUseSize;
	size_t DequeueSize;
	int iCnt;


	srand((unsigned int)GetThreadId(NULL));

#if VERSION == 0
	while (true)
#elif VERSION == 2
#endif
	{
		memset(dequeueBuf, 0, sizeof(dequeueBuf));
		rbUseSize = g_rb.GetUseSize();

		// 뽑을 데이터가 없다면 넘어간다.
		if (rbUseSize == 0)
		{
#if VERSION == 0
			continue;
#elif VERSION == 2
			return 0;
#endif
		}

		// 랜덤 최대값과 링버퍼 UseSize 중 더 작은 값을 랜덤하게 뽑는다. 
		RandomByteToExtract = rand() % (RAND_MAX > rbUseSize ? rbUseSize : RAND_MAX) + 1;

		DequeueSize = g_rb.Dequeue(dequeueBuf, rand() % RAND_MAX + 1);
		if (DequeueSize > 0)
		{
			for (iCnt = 0; iCnt < DequeueSize; ++iCnt)
			{
				printf("%c", dequeueBuf[iCnt]);
			}
		}

	}

	return 0;
}
