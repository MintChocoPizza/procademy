// hw12_echoClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")

//#define procademyserver

#ifdef procademyserver

#define DEFAULT_IP "procademyserver.iptime.org"
#define DEFAULT_PORT "10010"

#else

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "9000"

#endif // procademyserver


//#define DEFAULT_IP "211.51.91.75"
//#define DEFAULT_IP "127.0.0.1"
//#define DEFAULT_IP "192.168.10.14"
//#define DEFAULT_PORT "9000"

#define DEFAULT_BUFLEN 512

const wchar_t *DEFAULT_FILE_NAME = L"./CUCKOO.jpg";



struct st_PACKET_HEADER
{
	DWORD	dwPacketCode;		// 0x11223344	우리의 패킷확인 고정값

	WCHAR	szName[32];			// 본인이름, 유니코드 utf-16 NULL 문자 끝
	WCHAR	szFileName[128];	// 파일이름, 유니코드 utf-16 NULL 문자 끝
	int	iFileSize;
};



int main(int argc, char** argv)
{
	int i_Result;

	//----------------------------------
	// 전송할 파일 세팅 
	wchar_t w_File_Name[128];
	
	//----------------------------------
	// Initialize Winsock
	WSADATA wsa_Data;

	//----------------------------------
	// Setup hints
	struct addrinfo hints;

	//----------------------------------
	// Resolve the server address and port
	// 서버 주소 및 포트 확인
	struct addrinfo* result = NULL;

	//----------------------------------
	// Attempt to connect tot an address until one succeeds 
	// 성공할때 까지 주소에 연결을 시도한다.
	// 1. 서버 접속
	struct addrinfo* ptr = NULL;
	SOCKET Connect_Socket = INVALID_SOCKET;

	//----------------------------------
	// 2. 특정 파일 오픈
	FILE* p_File;
	long l_File_Size;
	char* p_File_Memory;

	//----------------------------------
	// 4. 헤더 생성
	struct st_PACKET_HEADER st_Packet_Header;

	//----------------------------------
	// send file
	int Bytes_Send;
	int Len_Send;
	
	//----------------------------------
	// Receive until the peer closes the connection
	char Recv_Buff[DEFAULT_BUFLEN];


	//----------------------------------
	// 전송할 파일 세팅 
	printf_s("전송할 파일을 입력하세요: ");
	if (fgetws(w_File_Name, sizeof(w_File_Name)/sizeof(wchar_t), stdin) != NULL) {
		// 문자열의 끝에 있는 '\n' 제거
		size_t len = wcslen(w_File_Name);
		
		// 비어있는 경우
		if (len == 1)
		{
			wcscpy_s(w_File_Name, DEFAULT_FILE_NAME);
		}
		else if (len > 0 && w_File_Name[len - 1] == '\n') {
			w_File_Name[len - 1] = '\0';
		}
	}
	printf_s("파일 입력 완료 \n");

	//----------------------------------
	// Initialize Winsock
	i_Result = WSAStartup(MAKEWORD(2, 2), &wsa_Data);
	if (i_Result != 0)
	{
		printf_s("WSAStartup failed with error: %d \n", i_Result);
		return 1;
	}

	//----------------------------------
	// Setup hints
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

	freeaddrinfo(result);

	if (Connect_Socket == INVALID_SOCKET)
	{
		printf_s("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	wprintf_s(L"Connected to server. \n");

	//----------------------------------
	// 2. 특정 파일 오픈
	i_Result = _wfopen_s(&p_File, w_File_Name, L"rb");
	if (i_Result != NULL || p_File == NULL)
	{
		printf_s("파일 불러오기 실패 \n");
		closesocket(Connect_Socket);
		WSACleanup();
		return -1;
	}
	// 3. 사이즈 체크
	fseek(p_File, 0, SEEK_END);
	l_File_Size = ftell(p_File);
	fseek(p_File, 0, SEEK_SET);

	p_File_Memory = (char*)malloc(l_File_Size);
	if (p_File_Memory == NULL)
	{
		printf_s("메모리 할당 실패 \n");
		closesocket(Connect_Socket);
		WSACleanup();
		return -1;
	}

	fread_s(p_File_Memory, l_File_Size, 1, l_File_Size, p_File);
	fclose(p_File);
	printf_s("특정 파일 오픈 성공 \n");

	//----------------------------------
	// 4. 헤더 생성
	st_Packet_Header.dwPacketCode = 0x11223344;
	wcscpy_s(st_Packet_Header.szName, L"최지혁");
	wcscpy_s(st_Packet_Header.szFileName, L"CUCKOO.jpg");
	st_Packet_Header.iFileSize = l_File_Size;

	//----------------------------------
	// 5. 서버로 헤더 전송
	i_Result = send(Connect_Socket, (char*)&st_Packet_Header, sizeof(st_PACKET_HEADER), 0);
	printf_s("헤더 전송 성공 \n");



	//----------------------------------
	// send file
	//i_Result = send(Connect_Socket, p_File_Memory, l_File_Size, 0);
	//if (i_Result == SOCKET_ERROR)
	//{
	//	printf_s("send failed with error: %d\n", WSAGetLastError());
	//	closesocket(Connect_Socket);
	//	WSACleanup();
	//	return 1;
	//}
	Bytes_Send = 0;
	Len_Send = 1000;
	do
	{
		// 전송해야할 데이터가 1000 보다 작다면 
		// 남은 데이터의 사이즈를 구하여, 그만큼만 전송한다. 
		if (l_File_Size - Bytes_Send < 1000)
			Len_Send = l_File_Size - Bytes_Send;
		i_Result = send(Connect_Socket, p_File_Memory+Bytes_Send, Len_Send, 0);
		if (i_Result == SOCKET_ERROR)
		{
			printf_s("send failed with error: %d\n", WSAGetLastError());
			closesocket(Connect_Socket);
			WSACleanup();
			return 1;
		}
		Bytes_Send += i_Result;
	} while (Bytes_Send < l_File_Size);
	printf_s("send file finish \n");

	


	//----------------------------------
	// Receive until the peer closes the connection
	do {

		i_Result = recv(Connect_Socket, Recv_Buff, sizeof(Recv_Buff), 0);
		if (i_Result > 0)
		{
			printf("Bytes received: %d\n", i_Result);
			printf_s("%x \n", *(int*)Recv_Buff);
			if (*(int*)Recv_Buff == 0xdddddddd)
			{
				printf_s("성공!!!!............. \n");
				break;
			}
			else
			{
				printf_s("실패!!!!............. \n");
				break;
			}
		}
		else if (i_Result == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (i_Result > 0);


	free(p_File_Memory);
	closesocket(Connect_Socket);
	WSACleanup();

	std::cout << "Hello World" << std::endl;
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
