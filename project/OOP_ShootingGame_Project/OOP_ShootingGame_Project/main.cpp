
#include <Windows.h>

#include "CScreenBuffer.h"
#include "CObjectManager.h"

#pragma comment (lib, "winmm.lib")

int main()
{
	timeBeginPeriod(1);
	CScreenBuffer* pCScreenBuffer = CScreenBuffer::GetInstance();



	while (1)
	{





		pCScreenBuffer->Flip();
	}

	timeEndPeriod(1);
	return 0;
}