// Ring_Buffer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <random>
#include <Windows.h>
#include <conio.h>

#include "C_Ring_Buffer.h"
#include "OSJ_Test.h"

#define df_TEST_DEQUEUE

#define _iSize 10
#define en_BUFFER_BLANK 4


using namespace std;

constexpr int MAX_SIZE = 81;

HANDLE h_Console;
const char* str = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";// 1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345 1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345 1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345 1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

size_t index = 0;
size_t Str_Len;
int En_Random;
int ret_En;

int ret_Pk;
char Buff_Pk[100];

int Dq_Random;
int ret_Dq;
char Buff_Dq[100];

char Buffer[100];

void test();
void test1();
void test2();

void cs_Initial(void);

int main()
{

	OSJ_test();
	

    std::cout << "Hello World!\n";
}

void git_test()
{
	cs_Initial();
	srand((unsigned int)time(NULL));
	C_RING_BUFFER* RingBuffer = new C_RING_BUFFER(100);
	char inbuff[] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

	char peekbuff[82];
	char outbuff[82];

	int size;
	int size2;

	int enqlen;
	int peeklen;
	int deqlen;

	printf("GetFreeSize	%d \n", RingBuffer->GetFreeSize());
	printf("GetUseSize	%d \n", RingBuffer->GetUseSize());
	printf("GetSize	%d \n", RingBuffer->GetBufferSize());
	//printf("GetUnbrokenDequeueSize	%d \n", RingBuffer->GetUnbrokenDequeueSize());
	//printf("GetUnbrokenEnqueueSize	%d \n", RingBuffer->GetUnbrokenEnqueueSize());




	//printf("\n\nBlank %d, Size %d \n", en_BUFFER_BLANK, _iSize);
	//printf("Write	Read	result	result2\n");
	//
	//int result, result2;
	//for (int ir = 0; ir < _iSize; ++ir)
	//{
	//	for (int iw = 0; iw < _iSize; ++iw)
	//	{
	//		if (ir > iw)
	//		{
	//			result = ir - (iw + en_BUFFER_BLANK);
	//			if (result < 0)
	//				result = 0;
	//		}
	//		else
	//		{
	//			if (ir < en_BUFFER_BLANK)
	//			{
	//				result = _iSize - iw - (en_BUFFER_BLANK - ir);
	//				if (result < 0)
	//					result = 0;
	//			}
	//			else
	//				result =  _iSize - iw;
	//		}


	//		/////////////////
	//		if ((iw + en_BUFFER_BLANK) % _iSize == ir)
	//		{
	//			result2 = 0;
	//			printf("!");
	//		}
	//		else if (iw <= ((ir + _iSize - en_BUFFER_BLANK) % _iSize))
	//		{
	//			result2 = ((ir + _iSize - en_BUFFER_BLANK) % _iSize) - iw;
	//			printf("@");
	//		}
	//		else if (iw >= ir)
	//		{
	//			result2 = _iSize - iw;
	//			printf("#");
	//		}
	//		else
	//		{
	//			result2 = 0;
	//			printf("$");
	//		}


	//		if (result != result2)
	//		{
	//			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	//			printf("%d	%d	%d	%d\n", iw , ir, result, result2);
	//			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	//		}
	//		else
	//			printf("%d	%d	%d	%d\n", iw, ir, result, result2);
	//	}
	//}
	//printf("끝\n\n");


	printf("\n");
	while (1)
	{
		size = rand() % 81 + 1;
		size2 = 81 - size;

		enqlen = RingBuffer->Enqueue(inbuff, sizeof(inbuff));
		enqlen = RingBuffer->Enqueue(inbuff, sizeof(inbuff));
		////////////////////////////////////////////////////
		// 1 임의의 크기 만큼 출력

		memset(outbuff, 0, sizeof(outbuff));
		memset(peekbuff, 0, sizeof(peekbuff));

		peeklen = RingBuffer->Peek(peekbuff, size);
		deqlen = RingBuffer->Dequeue(outbuff, size);

#ifdef df_TEST_DEQUEUE
		// peek과 dequeue 결과 비교
		if (memcmp(peekbuff, outbuff, size) == 0)
			printf(outbuff);
		else
			break;
#endif



		////////////////////////////////////////////////////
		// 2 남은 크기 마저 출력

		memset(outbuff, 0, sizeof(outbuff));
		memset(peekbuff, 0, sizeof(peekbuff));

		peeklen = RingBuffer->Peek(peekbuff, size2 + 10);	// 일부러 더 많이 뽑아보기	
		deqlen = RingBuffer->Dequeue(outbuff, size2 + 10);

#ifdef df_TEST_DEQUEUE
		// peek과 dequeue 결과 비교
		if (memcmp(peekbuff, outbuff, size2) == 0)
		{
			printf(outbuff);
		}
		else
			break;
#elif
		// 두번째 출력 데이터와 해당 부분인 입력 데이터 지점이 같은지 
		if (memcmp(peekbuff, inbuff + size, size2) == 0)
			printf(peekbuff);
		else
			break;
#endif

	}
	return;
}

void test1()
{
	cs_Initial();
	printf("%s \n\n", str);
	char c[100] = { 0, };
	while (1)
	{
		Str_Len = strlen(str + index)+1;
		En_Random = rand() % Str_Len;

		memcpy(c+index, str + index, En_Random);

		printf("%s", c);

		index += En_Random;

		if (index == strlen(str))
		{
			printf("%s", c);
			memset(c, 0, 100);
			index = 0;
		}



		getchar();
	}
}

void my_test2(void)
{
	cs_Initial();
	C_RING_BUFFER c_Ring_Buffer(100);
	srand((unsigned int)time(NULL));


	while (1)
	{
		memset(Buff_Pk, 0, 100);
		memset(Buff_Dq, 0, 100);

		Str_Len = strlen(str + index) + 1;

		En_Random = rand() % Str_Len;
		ret_En = c_Ring_Buffer.Enqueue(str + index, En_Random);
		//if (ret_En != En_Random) // 넣을 수 없는경우 안넣는다.
		//{
		//	__debugbreak();		// 인터럽트 3번을 발생한다. 
		//}
		index += En_Random;
		if (index == strlen(str))
		{
			index = 0;
		}





		Dq_Random = rand() % (strlen(str) + 1);
		ret_Pk = c_Ring_Buffer.Peek(Buff_Pk, Dq_Random, true);
		//if (ret_Pk != Dq_Random)
		//{
		//	__debugbreak();		// 인터럽트 3번을 발생한다. 
		//}

		ret_Dq = c_Ring_Buffer.Dequeue(Buff_Dq, Dq_Random);
		//if (ret_Dq != Dq_Random)
		//{
		//	__debugbreak();		// 인터럽트 3번을 발생한다. 
		//}

		if (memcmp(Buff_Pk, Buff_Dq, 100) != 0)
		{
			__debugbreak();		// 인터럽트 3번을 발생한다. 
		}

		printf("%s", Buff_Dq);

		//_getch();
	}
}

// 오승진씨 코드
void test3(void)
{
	
}

void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;
	char cCommand[28];

	//-----------------------------------------------------
	// 화면의 커서를 안보이게끔 설정한다.
	//-----------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;				// 커서 크기
	// 이상하게도 0이면 나온다. 1로 하면 안나온다.

	//-----------------------------------------------------
	// 콘솔화면 (스텐다드 아웃풋) 핸들을 구한다. 
	// 
	//-----------------------------------------------------
	h_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(h_Console, &stConsoleCursor);


	//-----------------------------------------------------
	// 콘솔화면 크기를 설정한다.
	//col = 가로, lines = 세로
	// 
	//-----------------------------------------------------
	sprintf_s(cCommand, "mode con:cols=%d lines=%d", 81, 40);

	system(cCommand);
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
