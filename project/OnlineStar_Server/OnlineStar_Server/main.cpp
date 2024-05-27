// OnlineStar_Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <map>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include "Init_Winsock_Server.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

//---------------------------------------------------
// 플레이어 리스트가 필요한 기능
// 1. 플레이어에게서 메시지가 들어오면 플레이어를 찾아야햠 
// 2. 리스트 순회
// 3. 


struct st_PLAYER
{
    sockaddr_in Socket_Addr;
    int ID;
    int X;
    int Y;
    //SOCKET Socket;
};

int g_ID = 1;

int main()
{
    //---------------------------------------------------
    // return 값 저장 변수 
    int Ret_select;
    bool Is_Init_Listen_Socket;
    

    //---------------------------------------------------
    //
    WSADATA wsa_Data;
    FD_SET Read_Set;
    FD_SET Write_Set;
    sockaddr_in Client_Addr;
    map<SOCKET, st_PLAYER> Player_List;
    map<SOCKET, st_PLAYER>::iterator iter_Player_List;
    int Client_Addr_Len = sizeof(Client_Addr);
    // TIMEVAL Time_Val;
    SOCKET Listen_Socket = INVALID_SOCKET;
    SOCKET Client_Socket;




    Is_Init_Listen_Socket = init_Listen_Socket(&Listen_Socket, &wsa_Data);
    if (Is_Init_Listen_Socket == false)
    {
        printf_s("소켓 초기화를 실패했습니다. \n");
        return -1;
    }

    while (1)
    {
        // 네트워크 
        FD_ZERO(&Read_Set);
        FD_SET(Listen_Socket, &Read_Set);
        for (iter_Player_List = Player_List.begin(); iter_Player_List != Player_List.end(); ++iter_Player_List)
        {
            FD_SET((*iter_Player_List).first, &Read_Set);
        }
        Ret_select = select(0, &Read_Set, 0, 0, NULL);
        if (Ret_select > 0)     // 사실 time_val == NULL 이라 무조건 무조건 처리할게 있다.
        {
            //---------------------------------------------------
            // 신규 접속 처리
            if (FD_ISSET(Listen_Socket, &Read_Set))
            {

            }

            //---------------------------------------------------
            // 메시지 처리
            for (iter_Player_List = Player_List.begin(); iter_Player_List != Player_List.end(); ++iter_Player_List)
            {
                if (FD_ISSET((*iter_Player_List).first, &Read_Set))
                {
                    
                }
            }


            
        }
        





        // 로직 -> 현재 서버는 따로 연산하는 값이 없다.

        // 랜더는 어떻게 할까?
    }


    //---------------------------------------------------
    // Listen_Socket을 해지한다. 
    closesocket(Listen_Socket);

    //---------------------------------------------------
    // WinSock 종료
    WSACleanup();

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


        //Client_Socket = accept(Listen_Socket, (SOCKADDR*)&Client_Socket, &Client_Addr_Len);
        //if (Client_Socket == INVALID_SOCKET)
        //{
        //    if (WSAGetLastError() != WSAEWOULDBLOCK)
        //    {
        //        printf_s("accpet failed with error: %ld \n", WSAGetLastError());
        //        return -1;
        //    }
        //}
