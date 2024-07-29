// Completion_Port_Model.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#pragma comment(lib, "ws2_32")
#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 25

// 소켓 정보 저장을 위한 구조체
struct st_SOCKETINFO
{
    OVERLAPPED overlapped;
    SOCKET sock;
    char buf[BUFSIZE + 1];
    int recvbytes;
    int sendbytes;
    WSABUF wsabuf;
};

// 수신
DWORD g_Recv_IO_PANDDING_COUNT = 0;
DWORD g_Recv_Synchronous_IO = 0;
// 송신
DWORD g_Send_IO_PANDDING_COUNT = 0;
DWORD g_Send_Synchronous_IO = 0;


// 작업자 스레드 함수
DWORD WINAPI WorkerThread(LPVOID arg);

// 오류 출력 함수

int main()
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    // 입출력 완료 포트 생성
    HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (hcp == NULL) return 1;

    // CPU 개수 확인 
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // (CPU 개수 * 2) 개의 작업자스레드 생성
    HANDLE hThread;
    for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
    {
        hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
        if (hThread == NULL) return 1;
        CloseHandle(hThread);
    }

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) printf("socket() \n");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    // 과제: 송신 버퍼 지우기
    // 
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int optval = 0;
    setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(optval));


    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) printf("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) printf("listen()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    DWORD recvbytes, flags;

    while (1)
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            printf("accept() \n");
            break;
        }
        printf("클라가 접속을 완료하였따. \n");

        // 소켓과 입출력 완료 포트 연결
        CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

        // 소켓 정보 구조체 할당
        st_SOCKETINFO* ptr = new st_SOCKETINFO;
        if (ptr == NULL) break;

        ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
        ptr->sock = client_sock;
        ptr->recvbytes = ptr->sendbytes = 0;
        ptr->wsabuf.buf = ptr->buf;
        ptr->wsabuf.len = BUFSIZE;

        // 비동기 입출력 시작 
        flags = 0;
        retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
        if (retval == SOCKET_ERROR)
        {
            int err = WSAGetLastError();

            if (err == ERROR_IO_PENDING)
            {
                InterlockedIncrement(&g_Recv_IO_PANDDING_COUNT);
            }

            if (err != ERROR_IO_PENDING)
            {
                printf("WSARecv()");
            }
            continue;
        }
        else
        {
            InterlockedIncrement(&g_Recv_Synchronous_IO);
        }
    }

    // 윈속 종료
    WSACleanup();

    std::cout << "Hello World!\n";
}

// 작업자 스레드 함수
DWORD WINAPI WorkerThread(LPVOID arg)
{
    int retval;
    HANDLE hcp = (HANDLE)arg;

    while (1)
    {
        // 비동기 입출력 완료 기다리기
        DWORD cbTransferred;
        SOCKET client_sock;
        st_SOCKETINFO* ptr;
        retval = GetQueuedCompletionStatus(hcp, &cbTransferred, &client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

        // 클라이언트 정보 얻기
        SOCKADDR_IN clientaddr;
        int addrlen = sizeof(clientaddr);
        getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

        // 비동기 입출력 결과 확인 
        if (retval == 0 || cbTransferred == 0)
        {
            if (retval == 0)
            {
                DWORD temp1, temp2;
                WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
                printf("WSAGetOverlappedResult()");
            }
            closesocket(ptr->sock);
            printf("서버 연결 종료 \n");
            delete ptr;
            continue;
        }

        printf("//---------------------------------\n");
        printf("SEND_IO_PANDDING: %d \n", g_Send_IO_PANDDING_COUNT);
        printf("SEND_Synchronous_ID: %d \n", g_Send_Synchronous_IO);
        printf("RECV_IO_PANDDING: %d \n", g_Recv_IO_PANDDING_COUNT);
        printf("RECV_Synchronous_IO: %d \n", g_Recv_Synchronous_IO);



        // 데이터 전송량 갱신
        if (ptr->recvbytes == 0)
        {
            ptr->recvbytes = cbTransferred;
            ptr->sendbytes = 0;

            // 받은 데이터 출력
            ptr->buf[ptr->recvbytes] = '\0';
            printf("받은 데이터 출력: %s \n", ptr->buf);
        }
        else
        {
            ptr->sendbytes += cbTransferred;
        }

        if (ptr->recvbytes > ptr->sendbytes)
        {
            // 데이터 보내기
            ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
            ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
            ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;


            DWORD sendbytes;
            retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
            if (retval == SOCKET_ERROR)
            {
                int err = WSAGetLastError();

                if (err == ERROR_IO_PENDING)
                {
                    InterlockedIncrement(&g_Send_IO_PANDDING_COUNT);
                }

                if (err != WSA_IO_PENDING)
                {
                    printf("WSASend()\n");
                }
                continue;
            }
            else
            {
                InterlockedIncrement(&g_Send_Synchronous_IO);
            }
        }
        else
        {
            ptr->recvbytes = 0;

            // 데이터 받기
            ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
            ptr->wsabuf.buf = ptr->buf;
            ptr->wsabuf.len = BUFSIZE;

            DWORD recvbytes;
            DWORD flags = 0;
            retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
            if (retval == SOCKET_ERROR)
            {
                int err = WSAGetLastError();

                if (err == ERROR_IO_PENDING)
                {
                    InterlockedIncrement(&g_Recv_IO_PANDDING_COUNT);
                }

                if (err != WSA_IO_PENDING)
                {
                    printf("WSARecv()\n");
                }
                continue;
            }
            else
            {
                InterlockedIncrement(&g_Recv_Synchronous_IO);
            }

        }


        printf("//---------------------------------\n");
        printf("SEND_IO_PANDDING: %d \n", g_Send_IO_PANDDING_COUNT);
        printf("SEND_Synchronous_ID: %d \n", g_Send_Synchronous_IO);
        printf("RECV_IO_PANDDING: %d \n", g_Recv_IO_PANDDING_COUNT);
        printf("RECV_Synchronous_IO: %d \n", g_Recv_Synchronous_IO);
    }
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
