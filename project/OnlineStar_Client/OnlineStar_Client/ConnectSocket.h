
#ifndef __CONNECT_SOCKET_H__
#define __CONNECT_SOCKET_H__

//--------------------------------------------------------------------
// 소켓의 포인터를 전달한다. 
// 
// 내부에서 SOCKET 로 형 변환하여 사용한다. 
//--------------------------------------------------------------------
bool connect_Server(void* Connect_Socket, void* wsa_Data);


#endif // !__CONNECT_SOCKET_H__

