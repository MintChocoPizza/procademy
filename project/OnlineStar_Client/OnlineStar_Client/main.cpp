// Force_RST_Transmission_Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include "Console.h"
#include "ConnectSocket.h"
#include "C_Queue.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;
using namespace OreoPizza;

struct st_PACKET
{
    int Type;
    int ID;
    int Y;
    int X;
};

struct st_PLAYER
{
    int ID;
    int Y;
    int X;
};

//---------------------------------------------------
// 전역으로 선연된 소켓
SOCKET Connect_Socket = INVALID_SOCKET;
WSADATA wsa_Data;

st_PLAYER *st_My_Player = NULL;



int main()
{
    FD_SET Read_Set;
    TIMEVAL Time_Val;
    C_Queue<st_PACKET> Recv_Buff(64);
    int i_Result;
    int i_Cnt;
    int i_Recv_Buff_Len; 
    bool Is_True;


    Is_True = connect_Server(&Connect_Socket, &wsa_Data);
    if (Is_True == false)
    {
        printf_s("연결을 실패했습니다. \n");
        return -1;
    }

    Recv_Buff = (unsigned char*)malloc(sizeof(st_PLAYER) * 64);
    i_Recv_Buff_Len = sizeof(st_PLAYER) * 64;
    
    while (1)
    {
        //---------------------------------------------------
        // 키보드 입력
        // 키 입력을 받는 함수 
        

        //---------------------------------------------------
        // 네트워크
        FD_ZERO(&Read_Set);
        FD_SET(Connect_Socket, &Read_Set);
        Time_Val.tv_sec = 0;
        Time_Val.tv_usec = 0;

        i_Result = select(0, &Read_Set, 0, 0, &Time_Val);
        if (i_Result > 0)
        {
            if (FD_ISSET(Connect_Socket, &Read_Set))
            {
                recv(Connect_Socket, Recv_Buff, )
            }
        }
        
        
        

        //---------------------------------------------------
        // 랜더
    }







    //---------------------------------------------------
    // 할당 받은 메모리를 해지한다.


    //---------------------------------------------------
    // Connect_Socket을 해지한다. 
    closesocket(Connect_Socket);

    //---------------------------------------------------
    // WinSock 종료
    WSACleanup();

    std::cout << "Hello World! " << std::endl;

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
