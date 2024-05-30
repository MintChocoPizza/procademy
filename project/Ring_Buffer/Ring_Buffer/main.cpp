// Ring_Buffer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <random>

#include "C_Ring_Buffer.h"

using namespace std;

int str_Index = 0;
int str_Cnt = 0;
const char* str = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345\
                   1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345\
                   1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345\
                   1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345\
                   1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";

C_RING_BUFFER c_Ring_Buffer(100);

int main()
{
    srand((unsigned int)time(NULL));

    size_t sz_Str = sizeof(str);

    int Eq_Size;
    int Eq_Rand_Num;

    int Dq_Rand_Num;

    int Pk_Size;
    char Pk_Buff[100];
    
    int Dq_Size;
    char Dq_Buff[100];

    int idx = 0;
    char Prt_Buff[100];

    while (1)
    {
        memset(Pk_Buff, 0, 100);
        memset(Dq_Buff, 0, 100);

        Eq_Rand_Num = rand() % sz_Str;
        str_Cnt += Eq_Rand_Num;
        {
            
        }
        Eq_Size = c_Ring_Buffer.Enqueue(str + str_Index, Eq_Rand_Num);
        if (Eq_Size != Eq_Rand_Num)
        {
            __debugbreak(); // 인터럽트 3번
        }

        Dq_Rand_Num = rand() % sz_Str;
        Pk_Size = c_Ring_Buffer.Peek(Pk_Buff, Dq_Rand_Num);
        if (Pk_Size != Dq_Rand_Num)
        {
            __debugbreak();
        }

        Dq_Size = c_Ring_Buffer.Dequeue(Pk_Buff, Dq_Rand_Num);
        if (Dq_Size != Dq_Rand_Num)
        {
            __debugbreak();
        }

        if (memcmp(Pk_Buff, Dq_Buff, 100) != 0)
        {
            __debugbreak();
        }

        

        
        
    }
    

    std::cout << "Hello World!\n";
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
