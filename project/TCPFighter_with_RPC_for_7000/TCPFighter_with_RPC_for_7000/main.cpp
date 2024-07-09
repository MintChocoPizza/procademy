
#include <iostream>
#include <time.h>
#include <conio.h>

#include <unordered_map>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "Ws2_32.lib")

//---------------------------------
// 시간 측정 변수
//---------------------------------
DWORD g_Start_Time;
DWORD g_End_Time;       // 매 루프마다 timeGetTime을 호출한다.
DWORD g_One_Second;

//---------------------------------
// 프로그램을 종료시키는 변수
//---------------------------------
bool g_bShutdown = false;


void ServerControl(void);
void Monitor(void);
void Update(void);

int wmain(int argc, wchar_t* argv[])
{
    timeBeginPeriod(1);
    srand((unsigned int)time(NULL));

    // 초기 시간을 세팅한다.
    g_Start_Time = timeGetTime();
    g_End_Time = g_Start_Time;

    // LoadData()                           // 설정 및 게임데이터, DB 데이터 로딩
    //netStartUp()                          // 네트워크 초기화, 리슨소켓 새엇ㅇ 및 listen  But Sesstion Class 의 싱글톤 생성자가 역할을 당담한다.

    while (!g_bShutdown)                               // 서버 메인 루프, 전역의 g_bShutdown 값에 의해 종료 결정   
    {
        // 네트워크 처리 함수()             // 네트워크 송수신 처리

        // 업데이트는 게임의 로직 (월드, 캐릭터, 몬스터, 이벤트 등...)
        // 실제 게임에서 돌아가야 하는 로직을 처리 한다. 

        Update();                           // 게임 로직 업데이트 

        ServerControl();                    // 키보드 입력을 통해서 서버를 제어할 경우 사용 
        Monitor();                          // 모니터링 정보를 표시, 저장, 전송하는 경우 사용
    }

    // 서버의 종료 대기 
    // 서버는 함부로 종료 해도 안된다. 
    // DB에 저장할 데이터나 기타 마무리 할 일들이 모두 끝났는지 확인한 뒤에 꺼주어야 한다. 

    // netCleanUp();                        // Sesstion class 싱글톤 소멸자가 메인함수 호출 뒤 atexit()에 의해 호출되어 처리된다. 



    std::cout << "Hello World!\n";
    return 0;
}


void ServerControl(void)
{
    // 개발, 디버깅, 긴급 등의 상황에서 사용할
    // 비공식적 기능, 특정 함수 호출, 기확자용 으로 사용된다. 

    // 기본적으로는 외부, 원격 네트워크등을 통하여 서버를 컨트롤 하게 된다.

    // 키보드 컨트롤 잠금, 풀림 변수 
    static bool bControlMode = false;

    //-------------------------------------------------------------------
    // L: 컨트롤 Lock  / U : 컨트롤 Unlock    / Q : 서버 종료
    //-------------------------------------------------------------------
    if (_kbhit())
    {
        __wchar_t ControlKey = _getwch();

        // 키보드 제어 허용 
        if (L'u' == ControlKey || L'U' == ControlKey)
        {
            bControlMode = true;

            // 관련 키 도움말 출력
            wprintf(L"Control Mode : Press Q - Quit \n");
            wprintf(L"Control Mode : Press L - Key Lock \n");
        }

        // 키보드 제어 잠금 
        if ((L'l' == ControlKey || L'L' == ControlKey) && bControlMode)
        {
            wprintf(L"Control Lock...! press U - Control Unlock \n");

            bControlMode = false;
        }


        // 키보드 제어 풀림 상태에서 특정 기능 
        if ((L'q' == ControlKey || L'Q' == ControlKey) && bControlMode)
        {
            g_bShutdown = true;
        }
    }
}

void Monitor(void)
{
}

void Update(void)
{

}
