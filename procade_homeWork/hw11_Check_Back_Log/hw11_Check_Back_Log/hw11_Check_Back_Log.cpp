// hw11_Check_Back_Log.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

using namespace std;

int main()
{

	// Winsock DLL 사용준비, Initialize Winsock
	WSADATA wsa_Data;
	int i_Result;

	// Resolve the server address and port
	// 서버 주소 및 포트 확인
	struct addrinfo* result = NULL;
	struct addrinfo hints;

	// Create a SOCKET for the server to listen for client connections.
	// 서버가 클라이언트 연결을 수신을 SOCKET을 만든다.
	SOCKET Listen_Socket;

	// Winsock DLL 사용준비, Initialize Winsock
	i_Result = WSAStartup(MAKEWORD(2, 2), &wsa_Data);
	if (i_Result != 0)
	{
		printf_s("WSAStartup failed with error: %d \n", i_Result);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	// Resolve the server address and port
	// 서버 주소 및 포트 확인
	i_Result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (i_Result != 0)
	{
		printf_s("getaddrinfo failed with error: %d \n", i_Result);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for the server to listen for client connections.
	// 서버가 클라이언트 연결을 수신을 SOCKET을 만든다.
	Listen_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Listen_Socket == INVALID_SOCKET)
	{
		printf_s("socket failed with error : %ld \n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	// TCP listening socket 을 준비한다.
	i_Result = bind(Listen_Socket, result->ai_addr, (int)result->ai_addrlen);
	if (i_Result == SOCKET_ERROR)
	{
		printf_s("bind failed with error: %d \n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(Listen_Socket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);
	// 소켓 준비 완료
	//-----------------------------------------------------------------------------

	// Listen a client socket
	i_Result = listen(Listen_Socket, SOMAXCONN);
	if (i_Result == SOCKET_ERROR)
	{
		printf_s("listen failed with error: %d \n", WSAGetLastError());
		closesocket(Listen_Socket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket





	WSACleanup();
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
