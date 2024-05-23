// Force_RST_Transmission_Client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

// #define DEFAULT_IP "127.0.0.1"
// #define DEFAULT_PORT "59812"
#define DEFAULT_BUFF_LEN 512

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int fags);

int main()
{
    int i_Result;

    //---------------------------------------------------
    // 포트와 ip를 입력하세요
    char IP[128];
    char PORT[128];

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
    struct addrinfo* ptr = NULL;
    SOCKET Connect_Socket = INVALID_SOCKET;
    //
    //----------------------------------
    // setsockopt_Linger
    struct linger Linger_Opt;
    //
    //----------------------------------
    // Connect to server

    //---------------------------------------------------
    // getaddrinfo() 에서 할당받은 메모리를 해지한다.

    // 소켓 연결 완료
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //---------------------------------------------------
    // 동작 선택
    int type;

    //---------------------------------------------------
    // 전송할 데이터 입력
    char Send_Buff[DEFAULT_BUFF_LEN];
    int Send_Len;

    //---------------------------------------------------
    // 데이터 받기
    char recv_buf[DEFAULT_BUFF_LEN];


    //---------------------------------------------------
    // Connect_Socket을 해지한다. 

    //---------------------------------------------------
    // WinSock 종료













    //---------------------------------------------------
    // 포트와 ip를 입력하세요
    printf_s("ip 입력하세요.\n예) 192.168.0.2 \n");
    scanf_s("%s", &IP, (unsigned)_countof(IP));
    printf_s("port 를 입력하세요 : ");
    scanf_s("%s", &PORT, (unsigned)_countof(PORT));

    








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
    i_Result = getaddrinfo(IP, PORT, &hints, &result);
    if (i_Result != 0)
    {
        printf_s("getadrinfo failed with error: %d \n", i_Result);
        WSACleanup();
        return -1;
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
            return -1;
        }

        //----------------------------------
        // setsockopt_Linger
        Linger_Opt.l_onoff = 1;
        Linger_Opt.l_linger = 0;
        i_Result = setsockopt(Connect_Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger_Opt, sizeof(Linger_Opt));
        if (i_Result == SOCKET_ERROR)
        {
            printf_s("setsockopt failed with error: %ld \n", WSAGetLastError());
            closesocket(Connect_Socket);
            WSACleanup();
            return -1;
        }

        //----------------------------------
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

    //---------------------------------------------------
    // getaddrinfo() 에서 할당받은 메모리를 해지한다.
    freeaddrinfo(result);

    if (Connect_Socket == INVALID_SOCKET)
    {
        // Connect_Socket == INVALID_SOCKET 인 경우는 무조건 closesocket(Connect_Socket) 되어있다.
        printf_s("Unable to connect to server \n");
        WSACleanup();
    }

    // 소켓 연결 완료
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (1)
    {

        while (getchar() != '\n'); // 입력 버퍼에서 개행 문자를 제거
        //fflush(stdin);

        //---------------------------------------------------
        // 동작 선택
        printf_s("1. 데이터 보내기 \n2. 데이터 받기\n3. 종료 \n");
        scanf_s("%d", &type);
        
        switch (type)
        {
        case 1:
        {
            //---------------------------------------------------
            // 전송할 데이터 입력
            printf_s("\n[보낼 데이터 파일 위치] ");
            if (fgets(Send_Buff, DEFAULT_BUFF_LEN, stdin) == NULL)
                break;


        }
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            printf_s("그딴 동작은 없습니다. 다시 입력하세요. \n");
            printf_s("1. 데이터 보내기 \n2. 데이터 받기\n3. 종료 \n");
            scanf_s("%d", &type);
            break;
        }
        
        //---------------------------------------------------
        // 전송할 데이터 입력
        printf_s("\n[보낼 데이터] ");
        if (fgets(Send_Buff, DEFAULT_BUFF_LEN, stdin) == NULL)
            break;

        // End 이면 프로그램 종료
        // 키보드 입력에는 '\n' 가 들어간다.
        if (strcmp(Send_Buff, "End\n") == 0)
            break;

        // 512를 입력하면 한번에 512 바이트를 전송한다.
        if (strcmp(Send_Buff, "512\n") == 0)
        {
            memset(Send_Buff, 0xff, DEFAULT_BUFF_LEN);
            Send_Buff[DEFAULT_BUFF_LEN - 1] = '\0';
        }

        // '\n' 문자 제거
        Send_Len = (int)strlen(Send_Buff);
        if (Send_Buff[Send_Len - 1] == '\n')
            Send_Buff[Send_Len - 1] = '\0';
        if (strlen(Send_Buff) == 0)
            break;

        // 데이터 보내기
        i_Result = send(Connect_Socket, Send_Buff, (int)strlen(Send_Buff), 0);
        if (i_Result == SOCKET_ERROR)
        {
            printf_s("send failed with error: %d\n", WSAGetLastError());
            closesocket(Connect_Socket);
            WSACleanup();
            return 1;
        }
        printf_s("Bytes Sent: %ld\n", i_Result);

        //---------------------------------------------------
        // 데이터 받기
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









    //---------------------------------------------------
    // Connect_Socket을 해지한다. 
    closesocket(Connect_Socket);

    //---------------------------------------------------
    // WinSock 종료
    WSACleanup();

    std::cout << "Hello World! " << std::endl;

    return 0;
}

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

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
