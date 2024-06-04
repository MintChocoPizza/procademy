// Message_Capture.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <map>
#include <iostream>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include "Init_Winsock_Server.h"

#pragma comment(lib, "Ws2_32.lib")

const char* DEFAULT_PORT = "25000";

using namespace std;

struct st_SOCKET
{
    SOCKET socket;
    sockaddr_in Socket_Addr;
};

int Ret_select;

SOCKET Listen_Socket = INVALID_SOCKET;
WSADATA wsa_Data;
FD_SET Read_Set;

map<SOCKET, st_SOCKET> Sock_List;

bool new_Connect(SOCKET* Listen_Socket);
void print_Message(const SOCKET* Client_Socket, sockaddr_in* Addr);

int main()
{
    bool Is_Init_Listen_Socket;
    Is_Init_Listen_Socket = init_Listen_Socket(&Listen_Socket, &wsa_Data);
    if (Is_Init_Listen_Socket == false)
    {
        printf_s("소켓 초기화를 실패했습니다. \n");
        return -1;
    }

    while (1)
    {
        // 연결 등록
        FD_ZERO(&Read_Set);
        FD_SET(Listen_Socket, &Read_Set);
        for (auto iter = Sock_List.begin(); iter != Sock_List.end(); ++iter)
        {
            FD_SET((*iter).first, &Read_Set);
        }

        Ret_select = select(0, &Read_Set, 0, 0, NULL);
        //---------------------------------------------------
        // 신규 접속 처리
        if (FD_ISSET(Listen_Socket, &Read_Set))
        {
            if (new_Connect(&Listen_Socket) == false)
            {
                // 유저 한명이 접속 못했으면 그냥 못한거다. 
                // 다른 행동을 하지 않는다.
            }
        }

        //---------------------------------------------------
        // 메시지 처리
        // 메시지를 받아서 모든 유저에게? 나를 제외한 모두에게? 전송한다.
        for (auto iter = Sock_List.begin(); iter != Sock_List.end(); ++iter)
        {
            if (FD_ISSET((*iter).first, &Read_Set))
            {
                print_Message(&(*iter).first, &(*iter).second.Socket_Addr);
            }
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

bool new_Connect(SOCKET* Listen_Socket)
{
    sockaddr_in Client_Addr;
    SOCKET Client_Socket;
    int Client_Addr_Len = sizeof(Client_Addr);
    u_long on;
    linger Linger_Opt;

    st_SOCKET st_Socket;
    char IP[32];



    //---------------------------------------------------
    // 반환값 저장
    int Ret_ioctlsocket;
    int Ret_setsockopt;

    //---------------------------------------------------
    // 나는 Client_Socket을 논블러킹 소켓으로 만든적이 없다 == 아마 상속되는거 같음 ?? 실제로 확인하기
    // accept로 연결에 실패하면 그냥 연결을 안하면 된다. 
    Client_Socket = accept(*Listen_Socket, (sockaddr*)&Client_Addr, &Client_Addr_Len);
    if (Client_Socket == INVALID_SOCKET)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf_s("accept failed with error: %d \n", WSAGetLastError());
            return false;
        }
    }

    //---------------------------------------------------
    // 넌블로킹 소켓으로 전환
    on = 1;
    Ret_ioctlsocket = ioctlsocket(Client_Socket, FIONBIO, &on);
    if (Ret_ioctlsocket == SOCKET_ERROR)
    {
        printf_s("ioctlsocket failed with error: %ld \n", WSAGetLastError());
        closesocket(Client_Socket);
        return false;
    }

    //---------------------------------------------------
    // setsockopt_Linger
    Linger_Opt.l_onoff = 1;
    Linger_Opt.l_linger = 0;
    Ret_setsockopt = setsockopt(Client_Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger_Opt, sizeof(Linger_Opt));
    if (Ret_setsockopt == SOCKET_ERROR)
    {
        printf_s("setsockopt failed with error: %ld \n", WSAGetLastError());
        closesocket(Client_Socket);
        return false;
    }

    
    st_Socket.socket = Client_Socket;
    st_Socket.Socket_Addr = Client_Addr;


    // 리스트에 등록
    Sock_List.insert(pair<SOCKET, st_SOCKET>(Client_Socket, st_Socket));



    inet_ntop(AF_INET, &Client_Addr.sin_addr, IP, sizeof(IP));
    printf("Conncet!! IP: %s, Port: %d \n", IP, Client_Addr.sin_port);

    return true;
}

void print_Message(const SOCKET* Client_Socket, sockaddr_in* Addr)
{

    //---------------------------------------------------
    // 반환값 저장
    int Ret_recv;
    int Ret_WSAGetLastError;

    char c_Buffer[10000];
    char IP[32];
    int i_Cnt;

    struct st_MESSAGE
    {
        unsigned short Len;
        int sx;
        int sy;
        int ex;
        int ey;
    };

    st_MESSAGE st_Message;

    inet_ntop(AF_INET, &Addr->sin_addr, IP, sizeof(IP));
    printf("\n\nIP: %s, Port: %d \n", IP, Addr->sin_port);
    //---------------------------------------------------
    // 받은 메시지를 출력한다
    Ret_recv = recv(*Client_Socket, c_Buffer, sizeof(c_Buffer), 0);
    if (Ret_recv == 0)
    {
    }
    else if (Ret_recv == -1)
    {
        Ret_WSAGetLastError = WSAGetLastError();
        if (Ret_WSAGetLastError == 10054)
        {
            printf_s("recv failed with error: %d\n", WSAGetLastError());
        }
        else if (Ret_WSAGetLastError == 10053)
        {
            printf_s("recv failed with error: %d\n", WSAGetLastError());
        }
        else if (Ret_WSAGetLastError == WSAEWOULDBLOCK)
        {
            printf_s("recv failed with error: %d\n", WSAGetLastError());
        }
        else
        {
            printf_s("recv failed with error: %d\n", WSAGetLastError());
            throw;
        }
    }
    else
    {
        for (i_Cnt = 0; i_Cnt < Ret_recv / sizeof(st_Message); i_Cnt += sizeof(st_Message))
        {
            memcpy(&st_Message, c_Buffer, sizeof(st_MESSAGE));


            // 주소, 포트: 메시지출력
            printf("sx: %d, sy: %d, ex: %d, ey: %d \n", st_Message.sx, st_Message.sy, st_Message.ex, st_Message.ey);
        }
    }


}
