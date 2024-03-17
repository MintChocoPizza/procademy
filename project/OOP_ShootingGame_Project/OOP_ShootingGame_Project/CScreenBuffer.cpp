
#include "CScreenBuffer.h"

CScreenBuffer* CScreenBuffer::GetInstance()
{
    return &CScreenBuffer::_CScreenbuffer;
}

void CScreenBuffer::Flip()
{
    int iCnt;
    for(iCnt = 0; iCnt < 24; ++iCnt)
    {

    }
}
