
#include <Windows.h>

#include "CScreenBuffer.h"

#include "CPlayerObject.h"

CPlayerObject::CPlayerObject(int ObjectType, int Y, int X) : CBaseObject(ObjectType, Y, X)
{

}

bool CPlayerObject::Update(void)
{
	KeyboardInput();

	return true;
}

void CPlayerObject::Render(void)
{
	DrawPlayer();
}

void CPlayerObject::KeyboardInput(void)
{
	// 왼쪽 이동 A.
	if (GetAsyncKeyState('A'))
	{
		_iX += -1;
	}
	// 오른쪽 이동 D.
	if (GetAsyncKeyState('D'))
	{
		_iX += 1;
	}
	// 위쪽 이동 W.
	if (GetAsyncKeyState('W') & 0x8001)
	{
		_iY -= 1;
	}
	// 아래쪽 이동 S.
	if (GetAsyncKeyState('S') & 0x8001)
	{
		_iY += 1;
	}
}

void CPlayerObject::DrawPlayer(void)
{
	CScreenBuffer* pCScreenBuffer = CScreenBuffer::GetInstance();
	pCScreenBuffer->SpriteDraw(_iY, _iX, '$');
}
