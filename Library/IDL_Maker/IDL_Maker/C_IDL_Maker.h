//--------------------------------------------------------------------------------------------------------------------
// 
// Attack()
// 
// Proxy.cpp
// 
//	최상단에 직렬화 버퍼와 네트워크 라이브러리가 물려 있어야 한다.
//	#include "직렬화 버퍼"
//	#include "네트워크 라이브러리"
// 
//	Attack()
//	{
//		1. 메시지 생성
//		2. Send(생성된 메시지를 넣는다).   -> 우리 네트워크 라이브러리의 Send 함수를 호출한다. 
//			Send는 어떻게 네트워크 라이브러리에서 보낼 사람을 구분하는가? 
//			SessionID를 무조건 받아야 한다.  -> 받을 사람 확정
//	}
// 
// Attack(SessionID *id, 메시지들 ~~~)
// {
//		1. 직렬화 버퍼 << 메 << 시 << 지 << 들;
//		
//		2. Send(SessionID, 생성된 메시지);
// }
// 네트워크 헤더 + 컨텐츠 헤더 + 컨텐츠 메시지
// 
//
// 
// -----------------------------------------------------------
// 현재 프로젝트에 간단하게 적용
// 
// 
// 
//		
// 
// 
//--------------------------------------------------------------------------------------------------------------------

#ifndef __C_IDL_MAKER_H__
#define	__C_IDL_MAKER_H__

class C_IDL_Maker
{
public:
	C_IDL_Maker();
	~C_IDL_Maker();

	void FileOpen(void);

public:

};

#endif // !__C_IDL_MAKER_H__
