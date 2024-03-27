
#include <stdio.h>

#include "CList.h"

#include "CParsing_ANSI.h"
#include "ConsoleBuffer.h"

#include "CBaseObject.h"
#include "CEnemy.h"

CEnemy::CEnemy(int ObjectType, char cSkin, int iHp, int iY, int iX, char *cMoveFile, char *GunFile) : CBaseObject(ObjectType), _Skin(cSkin), _Hp(iHp), _iY(iY), _iX(iX)
{
	GetMoving(cMoveFile);
	iter = MovingList.begin();
}

CEnemy::~CEnemy()
{
}

bool CEnemy::Update(void)
{
	Move();

	return true;
}

void CEnemy::Render(void)
{
	Moving temp = *iter;

	ConsoleBuffer::GetInstance()->Sprite_Draw(temp.dY, temp.dX, _Skin);
}

void CEnemy::GetMoving(char* cMoveFile)
{
	int iCnt;
	char cString[256];
	int iMaxMoving=0;

	Moving temp;

	CParsing_ANSI Parsing;

	Parsing.LoadFile(cMoveFile);
	Parsing.GetValue("MaxMoving", &iMaxMoving);

	for (iCnt = 1; iCnt <= iMaxMoving; ++iCnt)
	{
		sprintf_s(cString, "MovingCool%d", iCnt);
		Parsing.GetValue(cString, &temp._CoolTime);

		sprintf_s(cString, "MovingY%d", iCnt);
		Parsing.GetValue(cString, &temp.dY);

		sprintf_s(cString, "MovingX%d", iCnt);
		Parsing.GetValue(cString, &temp.dX);

		MovingList.push_back(temp);
	}
}

void CEnemy::Move(void)
{
	++iter;

	if (iter == MovingList.end())
		iter = MovingList.begin();
}

