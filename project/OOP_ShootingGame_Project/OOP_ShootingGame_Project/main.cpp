

#include "CScreenBuffer.h"
#include "CPlayerObject.h"


int main()
{
	CScreenBuffer* pCScreenBuffer = CScreenBuffer::GetInstance();

	CPlayerObject aaa(12, 12, 12);


	while (1)
	{
		aaa.Update();

		aaa.Render();

		pCScreenBuffer->Flip();
	}

	return 0;
}