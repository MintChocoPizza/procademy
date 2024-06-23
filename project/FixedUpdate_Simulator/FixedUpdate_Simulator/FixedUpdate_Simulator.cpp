// FixedUpdate_Simulator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

constexpr int TICK_PER_SECONDS      =   1000;       // 틱 당 초
constexpr int TICK_PER_FRAME        =   20;         // 틱 당 프레임 
constexpr int OVERHEAD              =   900;        // 오버헤드 == 부하 

// #define MAXDELTA
// #define PRINT

#ifdef MAXDELTA
INT maxDeltaTime = 4000;
#endif // MAXDELTA

DOUBLE timeScale = 1;

int g_FixedUpdateCounter;
int g_UpdateCounter = 0;

void updateTime(int* unscaledTime, int* time)
{
    //-----------------------------------------------------------------------------------------------
    int unscaled_deltaTime = timeGetTime() - *unscaledTime;
    int deltaTime = unscaled_deltaTime * timeScale;

#ifdef MAXDELTA
    if (deltaTime > maxDeltaTime)
        deltaTime = maxDeltaTime;
#endif // MAXDELTA



    //-----------------------------------------------------------------------------------------------
    // timeScale이 2가 되면 *time(조정된 시간)이 더 커진다.
    * unscaledTime += unscaled_deltaTime;
    *time += deltaTime;
}


int OSJ_FixedUpdate(void)
{
    bool timeScaleFlag = false; 
    bool overHeadFlag = true;
    timeBeginPeriod(1);
    int Rendering_FPS = 0;

    constexpr int fixedDeltaTime = TICK_PER_SECONDS / TICK_PER_FRAME;

    srand(time(NULL));

    int initialTime = timeGetTime();

    // 1초가 지나갈때마다 표시하기 위한 변수 
    int timeForSecond = initialTime;

    // timeScale의 영향을 받지 않는 실제시간, 즉 unscaledTimeStemp
    int unscaledTime = initialTime;

    // 마지막 FixedUpdate가 호출된 시간 즉 FixedTimeStemp
    int FixedTime = initialTime;

    // timeScale의 영향을 받는 시간으로(timeStemp) 한번에 최대 maxDeltatime만큼만 증가한다. 
    int time = initialTime;

    

    while (1)
    {
        // --- 로직 ---
        if (GetAsyncKeyState(VK_RSHIFT) & 0x8001)
            timeScaleFlag = !timeScaleFlag;

        if (GetAsyncKeyState(VK_CONTROL) & 0x8001)
            overHeadFlag = !overHeadFlag;

        if (timeScaleFlag)
            timeScale = 2;

        if (!overHeadFlag)
            Sleep(rand() % 3000);

        for (int i = 0; i < 10000000; ++i);


        //--------------------------------------------------------------------------------------------
        // timeScale에 따라 조절되는 fixedUpdate 호출 주기 
        // 기본 timeScale은 1임 --> FCI == fixedDeltaTime 같음
        //--------------------------------------------------------------------------------------------
        int FCI = fixedDeltaTime / timeScale;

        updateTime(&unscaledTime, &time);

        // 설정한 fixedUpdate 호출 주기에 비해 프레임이 밀리는 경우
        // unscaledTimeStemp 와 fixedTimeStemp가 FCI 이상 벌어지면 FixedUpdate 수행
        while (unscaledTime - FixedTime >= FCI)
        {
            ++g_FixedUpdateCounter;
            FixedTime += FCI;       // fixedTimeStep 갱신
#ifdef PRINT
            printf("FixedUpdateCount: %d \n", g_FixedUpdateCounter);
#endif // PRINT
        }


        // Update 수행 
        ++g_UpdateCounter;

#ifdef PRINT
        printf("UpdateCount : %d \n", g_UpdateCounter);
#endif // PRINT

        // Rendering 수행
        ++Rendering_FPS;



        // 시간 경과 및 FPS를 측정하기 위한 용도 그 이상도 이하도 아님
        int temp = timeGetTime();
        if (temp - timeForSecond >= TICK_PER_SECONDS)
        {
            int delta = (temp - timeForSecond);

            // 1초 이상 밀린 경우 시간 표시자 업데이트 ex: 3500ms 밀리면 1000ms씩 3번 더해줌 
            while (delta >= TICK_PER_SECONDS)
            {
                timeForSecond += TICK_PER_SECONDS;
                delta -= TICK_PER_SECONDS;
            }
            updateTime(&unscaledTime, &time);

            printf("FixedUpdateFPS : %2d, \
                    UpdateFPS : %2d, \
                    Rendering_FPS : %2d, \
                    timeScale : %f, \
                    time : %2d, \
                    unscaled time : %2d \n\n\n\n", 
                    g_FixedUpdateCounter, g_UpdateCounter, Rendering_FPS, timeScale, time / 1000, unscaledTime / 1000);
        }


        // 타임스케일은 프레임마다 1로 초기화 된다. 
        timeScale = 1;
    }

    timeEndPeriod(1);
    return 0;
}

void FixedUpdate(void)
{

}

int main()
{
    OSJ_FixedUpdate();

    std::cout << "Hello World \n";
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
