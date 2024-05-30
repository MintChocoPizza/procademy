
#include <string.h>

#include "C_Ring_Buffer.h"

/////////////////////////////////////////////////////////////////////////
// 기본 사이즈를 1만 Byte로 할당한다.
/////////////////////////////////////////////////////////////////////////
inline C_RING_BUFFER::C_RING_BUFFER(void) : _In(0), _Out(0), _Full_Size(10000)
{
	_Buffer = new char[10000];

	_Buffer_End = _Buffer + 10000;
}

C_RING_BUFFER::C_RING_BUFFER(int i_Buffer_Size) : _In(0), _Out(0), _Full_Size(10000)
{
	_Buffer = new char[i_Buffer_Size];

	_Buffer_End = _Buffer + i_Buffer_Size;
}

/////////////////////////////////////////////////////////////////////////
// 현재 버퍼의 최대 크기
// 
// Parameters: 없음.
// Return: (int)버퍼 최대 용량.
/////////////////////////////////////////////////////////////////////////
int C_RING_BUFFER::GetBufferSize(void)
{
	return _Full_Size;
}

/////////////////////////////////////////////////////////////////////////
// 현재 사용중인 용량 얻기.
//
// Parameters: 없음.
// Return: (int)사용중인 용량.
/////////////////////////////////////////////////////////////////////////
int C_RING_BUFFER::GetUseSize(void)
{
	return _Use_Size;
}

/////////////////////////////////////////////////////////////////////////
// 현재 버퍼에 남은 용량 얻기. 
//
// Parameters: 없음.
// Return: (int)남은용량.
/////////////////////////////////////////////////////////////////////////
int C_RING_BUFFER::GetFreeSize(void)
{
	return _Free_Size;
}

/////////////////////////////////////////////////////////////////////////
// WritePos 에 데이타 넣음.
// 남은 버퍼의 크기가 iSize보다 작은 경우 데이터를 넣지 않고 0을 반환한다.
//
// Parameters: (char *)데이타 포인터. (int)크기. 
// Return: (int)넣은 크기.
/////////////////////////////////////////////////////////////////////////
int C_RING_BUFFER::Enqueue(const char* pData, int iSize)
{
	int Data_Chunk_Size;
	char* Temp_In = _In;

	if(_Free_Size < iSize)
		return 0;

	// (_Buffer + _Full_Size): 마지막 버퍼 그 다음 위치
	// (_Buffer + Full_Size) - _In : 현재 위치에서 마지막 버퍼까지 넣을 수 있는 데이터의 수가 나온다. 
	Data_Chunk_Size = _Buffer_End - Temp_In;
	
	if(iSize <= Data_Chunk_Size)
	{
		memcpy(Temp_In, pData, iSize);
		_In = (char*)(((intptr_t)Temp_In + iSize) % _Full_Size);
	}
	else
	{
		memcpy(Temp_In, pData, Data_Chunk_Size);
		memcpy(_Buffer, pData + Data_Chunk_Size, iSize - Data_Chunk_Size);
		_In = (char*)(((intptr_t)Temp_In + iSize) % _Full_Size);
	}

	return iSize;
}



