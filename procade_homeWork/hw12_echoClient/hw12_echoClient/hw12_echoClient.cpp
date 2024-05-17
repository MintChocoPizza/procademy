// hw12_echoClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

//#define DEFAULT_IP "211.51.91.75"
#define DEFAULT_IP "127.0.0.1"
//#define DEFAULT_IP "192.168.10.14"
#define DEFAULT_PORT "9000"
#define DEFAULT_BUFLEN 512

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int fags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, fags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}


int main(int argc, char** argv)
{
	// Initialize Winsock
	// Winsock 초기화
	WSADATA wsa_Data;
	int i_Result;

	// Setup hints
	struct addrinfo hints;

	// Resolve the server address and port
	// 서버 주소 및 포트 확인
	struct addrinfo* result = NULL;

	// Attempt to connect tot an address until one succeeds 
	// 성공할때 까지 주소에 연결을 시도한다.
	struct addrinfo* ptr = NULL;
	SOCKET Connect_Socket = INVALID_SOCKET;

	// Send an initial buffer
	char send_buf[DEFAULT_BUFLEN];
	int send_len;

	// Receive until the peer closes the connection
	char recv_buf[DEFAULT_BUFLEN];
	int recv_buf_len = DEFAULT_BUFLEN;



	// Initialize Winsock
	// Winsock 초기화
	i_Result = WSAStartup(MAKEWORD(2, 2), &wsa_Data);
	if (i_Result != 0)
	{
		printf_s("WSAStartup failed with error: %d \n", i_Result);
		return 1;
	}

	// Setup hints
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	if (argc == 2)
		i_Result = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	else
		i_Result = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
	if (i_Result != 0)
	{
		printf_s("getadrinfo failed with error: %d \n", i_Result);
		WSACleanup();
		return 1;
	}

	// connect()
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

	freeaddrinfo(result);

	if (Connect_Socket == INVALID_SOCKET)
	{
		printf_s("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	while (1)
	{
		// 데이터 입력
		printf_s("\n[보낼 데이터] ");
		if (fgets(send_buf, DEFAULT_BUFLEN, stdin) == NULL)
			break;

		// '\n' 문자 제거
		send_len = strlen(send_buf);
		if (send_buf[send_len - 1] == '\n')
			send_buf[send_len - 1] = '\0';
		if (strlen(send_buf) == 0)
			break;

		// 데이터 보내기
		i_Result = send(Connect_Socket, send_buf, (int)strlen(send_buf), 0);
		if (i_Result == SOCKET_ERROR) 
		{
			printf_s("send failed with error: %d\n", WSAGetLastError());
			closesocket(Connect_Socket);
			WSACleanup();
			return 1;
		}
		printf_s("Bytes Sent: %ld\n", i_Result);

		// 데이터 받기
		bool isEnd = false;
		// i_Result = recv(Connect_Socket, recv_buf, recv_buf_len, MSG_WAITALL);
		i_Result = recvn(Connect_Socket, recv_buf, i_Result, 0);
		if (i_Result > 0)
		{
			recv_buf[i_Result] = '\0';
			printf_s("[받은 데이터] %s \n", recv_buf);
			printf_s("Bytes received: %d\n", i_Result);

		}
		else if (i_Result == 0)
		{
			printf_s("Connection closed\n");
			closesocket(Connect_Socket);
			WSACleanup();
			return 1;
		}
		else
		{
			printf_s("recv failed with error: %d\n", WSAGetLastError());
			closesocket(Connect_Socket);
			WSACleanup();
			return 1;
		}
	}


	i_Result = shutdown(Connect_Socket, SD_SEND);
	if (i_Result == SOCKET_ERROR) {
		printf_s("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(Connect_Socket);
		WSACleanup();
		return 1;
	}

	std::cout << "Hello World \n" << std::endl;
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
