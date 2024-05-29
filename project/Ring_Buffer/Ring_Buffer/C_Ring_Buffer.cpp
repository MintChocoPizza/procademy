
#include "C_Ring_Buffer.h"

/////////////////////////////////////////////////////////////////////////
// 기본 사이즈를 1만 Byte로 할당한다.
/////////////////////////////////////////////////////////////////////////
template<class Data>
inline C_RING_BUFFER<Data>::C_RING_BUFFER(void) : _In(0), _Out(0), _Full_Size(10000)
{
	_Buffer = new Data[10000];
}

template<class Data>
C_RING_BUFFER<Data>::C_RING_BUFFER(int i_Buffer_Size) : _In(0), _Out(0), _Full_Size(10000)
{
	_Buffer = new Data[i_Buffer_Size];
}

/////////////////////////////////////////////////////////////////////////
// 현재 버퍼의 최대 크기
// 
// Parameters: 없음.
// Return: (int)버퍼 최대 용량.
/////////////////////////////////////////////////////////////////////////
template<class Data>
int C_RING_BUFFER<Data>::GetBufferSize(void)
{
	return _Full_Size;
}

/////////////////////////////////////////////////////////////////////////
// 현재 사용중인 용량 얻기.
//
// Parameters: 없음.
// Return: (int)사용중인 용량.
/////////////////////////////////////////////////////////////////////////
template<class Data>
int C_RING_BUFFER<Data>::GetUseSize(void)
{
	return _Use_Size;
}

/////////////////////////////////////////////////////////////////////////
// 현재 버퍼에 남은 용량 얻기. 
//
// Parameters: 없음.
// Return: (int)남은용량.
/////////////////////////////////////////////////////////////////////////
template<class Data>
int C_RING_BUFFER<Data>::GetFreeSize(void)
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
template<class Data>
int C_RING_BUFFER<Data>::Enqueue(const char* chpData, int iSize)
{
	if (_Free_Size == 0)
		return 0;

	if (_Free_Size < iSize)
		return 0;

	
	

	return 0;
}
template<class Data>
int C_RING_BUFFER<Data>::Enqueue(const Data* pData, int iSize)
{
	return 0;
}



