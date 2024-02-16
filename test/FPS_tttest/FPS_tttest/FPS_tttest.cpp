// FPS_tttest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <Windows.h>
#include <time.h>

#pragma comment (lib, "winmm.lib")

DWORD One_Seconde_Timer;
DWORD Virtual_Time;
DWORD Real_Current_Time;


int logic = 0;
int rander = 0;
int load = 10;


void Input(void);
void Logic(void);
void Rander(void);
void Skip(void);

//--------------------------------------------------------------
// 50fps -> 1초에 50번 
// 
// while문 한번 실행하는데 1000/50 => 20ms
//--------------------------------------------------------------
int main()
{
    timeBeginPeriod(1);
  
    //QueryPerformanceCounter(&Start);
    Virtual_Time = GetTickCount();
    One_Seconde_Timer = Virtual_Time; 
    while (1)
    {
        // 키보드 입력
        Input();

        // 로직 
        Logic();

        // 프레임 스킵 
        // 랜더링

            Rander();
            Skip();
    }

    timeEndPeriod(1);
    return 0;
}

void Input(void)
{
    if (GetAsyncKeyState(VK_SPACE) & 0x8001)
    {
        load += 1;
    }
}


void Logic(void)
{
    Sleep(load + rand() % 5);
    logic++;
}
void Rander(void)
{
    Sleep(load + rand() % 10);
    rander++;
    
}
void Skip(void)
{
    //bool ret = true;
    Real_Current_Time = GetTickCount();


    //------------------------------------------------------------------------
    // 목표 프레임: 50FPS
    // 1초에 50번의 while문
    // 
    // while문 한번에 20ms
    // 
    // =====>> while문 한번에 랜더링 + 로직이 돌아야 한다. 
    // ====>> 만약 1FPS를 초과 했다면 랜더링을 포기하고 로직을 돌린다. 
    // 
    // 현재 측정한 시간에서 가상의 시간을 뺌
    // - 딱 맞으면 0
    // - 가상 시간이 빠르게 끝났으면 20ms보다 작아야 한다. 
    // 
    //------------------------------------------------------------------------
    if (Real_Current_Time - Virtual_Time < 20)       // 남은 시간동안 Sleep()
    {

        Sleep(20 - (Real_Current_Time - Virtual_Time));
        printf_s("슬립실행 %d \n", 20 - (Real_Current_Time - Virtual_Time));

        //ret = true;
    }
    else if(Real_Current_Time - Virtual_Time > 20)
    {
        //ret = false;       // 20(1프레임) 이상 차이가 난다면 랜더링을 포기하고 로직을 돌린다.
    }
    printf_s(" %d - %d   ================= %d ======================= \n\n", Real_Current_Time, Virtual_Time, Real_Current_Time - Virtual_Time);

    Virtual_Time += 20;

    


    // 1초가 지났다면 로직과 랜더링을 확인한다.
    if (Real_Current_Time - One_Seconde_Timer >= 1000 )
    {
        One_Seconde_Timer += 1000;
        printf_s("로직: %d - 랜더: %d \n", logic, rander);
        rander = 0;
        logic = 0;
    }
    // return ret;
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
