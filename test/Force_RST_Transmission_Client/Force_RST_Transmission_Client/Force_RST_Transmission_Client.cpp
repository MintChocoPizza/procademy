// Force_RST_Transmission_Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "9000"

int main()
{
    int i_Result;

    //---------------------------------------------------
    // initialize Winsock
    WSADATA wsa_Data;

    //---------------------------------------------------
    // SetUp hints 
    struct addrinfo hints;

    //----------------------------------
    // Resolve the server address and port
    // 서버 주소 및 포트 확인
    struct addrinfo* result = NULL;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //----------------------------------
	// Attempt to connect tot an address until one succeeds 
	// 성공할때 까지 주소에 연결을 시도한다.
    // 1. 서버 접속
    struct addrinfo* ptr = NULL;
    SOCKET Connect_Socket = INVALID_SOCKET;
    //----------------------------------
    // 






    //---------------------------------------------------
    // initialize Winsock
    i_Result = WSAStartup(MAKEWORD(2, 2), &wsa_Data);
    if (i_Result != 0)
    {
        printf_s("WSAStartup failed with error: %d \n", i_Result);
        return -1;
    }

    //---------------------------------------------------
    // SetUp hints 
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //----------------------------------
    // Resolve the server address and port
    // 서버 주소 및 포트 확인
    i_Result = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
    if (i_Result != 0)
    {
        printf_s("getadrinfo failed with error: %d \n", i_Result);
        WSACleanup();
        return 1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //----------------------------------
    // Attempt to connect tot an address until one succeeds 
    // 성공할때 까지 주소에 연결을 시도한다.
    // 1. 서버 접속
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
        Connect_Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (Connect_Socket == INVALID_SOCKET)
        {
            printf_s("socket failed with error: %ld \n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server
        i_Result = connect(Connect_Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (i_Result == SOCKET_ERROR)
        {
            closesocket(Connect_Socket);
            Connect_Socket = INVALID_SOCKET;
            continue;
        }
        break;
    }






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
