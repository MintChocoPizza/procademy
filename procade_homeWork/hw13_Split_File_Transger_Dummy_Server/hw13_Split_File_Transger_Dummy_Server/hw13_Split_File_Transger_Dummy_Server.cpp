// hw12_echoServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define DEFAULT_PORT "9000"
#define DEFAULT_BUFF_LEN 512

using namespace std;

struct st_PACKET_HEADER
{
	DWORD	dwPacketCode;		// 0x11223344	우리의 패킷확인 고정값

	WCHAR	szName[32];			// 본인이름, 유니코드 utf-16 NULL 문자 끝
	WCHAR	szFileName[128];	// 파일이름, 유니코드 utf-16 NULL 문자 끝
	int	iFileSize;
};

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

	// Accept a client socket
	// client socket 수락
	SOCKET Client_Socket;
	sockaddr_in client;
	int client_Size = sizeof(client);


	// 클라이언트 ip : port 출력
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	// 파일 헤더 수신 
	struct st_PACKET_HEADER st_Packet_Header;
	int Bytes_Received_Total;

	// 파일 열기
	FILE* p_File;
	wchar_t File_Name[256] = L"";

	// 파일 수신
	int Bytes_Received;
	char recv_buf[DEFAULT_BUFF_LEN];
	int recv_buf_len = DEFAULT_BUFF_LEN;

	// 응답 보내기
	int i_Send_Result;
	int Send_Data = 0xdddddddd;
	int Send_Data_Len = sizeof(Send_Data);





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

	// Listen a client socket
	i_Result = listen(Listen_Socket, SOMAXCONN);
	//i_Result = listen(Listen_Socket, SOMAXCONN_HINT(65535));	// 양수 ~200 -> 기본 200개, 200~ 갯수 적용,    
	if (i_Result == SOCKET_ERROR)
	{
		printf_s("listen failed with error: %d \n", WSAGetLastError());
		closesocket(Listen_Socket);
		WSACleanup();
		return 1;
	}


	// while (1)
	{
		// accept()
		printf_s("\naccept를 기다림.....");
		Client_Socket = accept(Listen_Socket, (sockaddr*)&client, &client_Size);
		if (Client_Socket == INVALID_SOCKET)
		{
			printf_s("accept failed with error: %d \n", WSAGetLastError());
			closesocket(Listen_Socket);
			WSACleanup();
			return 1;
		}

		// 접속한 클라이언트 정보 출력
		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			std::cout << host << " : " << service << "에 연결되었습니다." << std::endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			printf_s("%s : %d 에 연결되었습니다. \n", host, ntohs(client.sin_port));
		}




		// 파일 헤더 수신
		Bytes_Received_Total = 0;
		do
		{
			Bytes_Received = recv(Client_Socket, (char*)&(st_Packet_Header)+Bytes_Received_Total, 1, 0);
			Bytes_Received_Total += Bytes_Received;
		} while (Bytes_Received_Total < sizeof(st_PACKET_HEADER));
		// Bytes_Received = recv(Client_Socket, (char*)&st_Packet_Header, sizeof(st_Packet_Header), 0);

		// 수신 헤더 확인
		printf_s("dwPacketCode: %x \n", st_Packet_Header.dwPacketCode);
		wprintf_s(L"szName: %s \n", st_Packet_Header.szName);
		wprintf_s(L"szFileName: %s \n", st_Packet_Header.szFileName);
		printf_s("iFileSize: %d \n", st_Packet_Header.iFileSize);

		// 파일 열기
		wcscat_s(File_Name, st_Packet_Header.szName);
		wcscat_s(File_Name, L"_");
		wcscat_s(File_Name, st_Packet_Header.szFileName);

		i_Result = _wfopen_s(&p_File, File_Name, L"ab");
		if (i_Result != NULL || p_File == NULL)
		{
			printf_s("파일 불러오기 실패 \n");
			closesocket(Client_Socket);
			closesocket(Listen_Socket);

			// WinSock 종료
			WSACleanup();

			std::cout << "Hello World!\n";
			return 0;
		}
		printf_s("파일 오픈 성공...... \n");

		// 파일 수신
		Bytes_Received_Total = 0;
		do
		{
			Bytes_Received = recv(Client_Socket, recv_buf, recv_buf_len, 0);
			fwrite(recv_buf, 1, Bytes_Received, p_File);
			Bytes_Received_Total += Bytes_Received;
		} while (Bytes_Received_Total <st_Packet_Header.iFileSize);
		fclose(p_File);
		printf_s("파일 수신 완료 \n");

		// 응답 보내기

		i_Send_Result = send(Client_Socket, (char*)&Send_Data, Send_Data_Len, 0);
		if (i_Send_Result == SOCKET_ERROR) {
			printf_s("send failed with error: %d\n", WSAGetLastError());
			closesocket(Client_Socket);
			WSACleanup();
			return 1;
		}

	}

	// listen socket을 해지한다. 
	closesocket(Listen_Socket);

	// WinSock 종료
	WSACleanup();

	std::cout << "Hello World!\n";
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
