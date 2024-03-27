
#include <Windows.h>


#include "ConsoleBuffer.h"

#include "CBaseObject.h"
#include "CPlayer.h"

CPlayer::CPlayer(int ObjectType, int iY, int iX, char cSkin) : CBaseObject(ObjectType), _iY(iY), _iX(iX), _cSkin(cSkin)
{


}

CPlayer::~CPlayer()
{

}

bool CPlayer::Update(void)
{
	KeyboardInput();

	ConsoleBuffer::GetInstance()->Sprite_Draw(_iY, _iX, _cSkin);

	return true;
}

void CPlayer::Render(void)
{
	ConsoleBuffer::GetInstance()->Buffer_Flip();
}

void CPlayer::KeyboardInput(void)
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
	//-------------------------------------------------------------
	// 플레이어 이동 반경 제한.
	// 게임 화면에서 플레이어가 이동 가능한 구역을 제한한다.
	//-------------------------------------------------------------

	_iX = max(_iX, 0);
	_iX = min(_iX, 79);
	_iY = max(_iY, 0);
	_iY = min(_iY, 23);
}
