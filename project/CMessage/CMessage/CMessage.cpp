
#include <Windows.h>

#include "CMessage.h"

CMessage::CMessage()
{
	m_chpBuffer = new char[eBUFFER_DEFAULT];
	m_iBufferSize = eBUFFER_DEFAULT;
	m_iDataSize = 0;
	m_chpRead = m_chpWrite = m_chpBuffer;
}

CMessage::CMessage(int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	m_iBufferSize = eBUFFER_DEFAULT;
	m_iDataSize = 0;
	m_chpRead = m_chpWrite = m_chpBuffer;
}

CMessage::~CMessage()
{
	delete[] m_chpBuffer;
}

CMessage& CMessage::operator=(CMessage& clSrcPacket)
{
	// TODO: 여기에 return 문을 삽입합니다.

}

CMessage& CMessage::operator<<(unsigned char byValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

}

CMessage& CMessage::operator<<(char chValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

}
