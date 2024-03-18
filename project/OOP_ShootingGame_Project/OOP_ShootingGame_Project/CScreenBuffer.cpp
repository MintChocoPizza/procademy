#include "CScreenBuffer.h"

#include <stdio.h>
#include <cstring>

CScreenBuffer CScreenBuffer::_CScreenBuffer;

CScreenBuffer::CScreenBuffer() : hConsole(0)
{
	memset(ScreenBuffer, 0, sizeof(ScreenBuffer));
	Initial();
	printf_s("스크린 버퍼 생성자 동작 \n");
}

CScreenBuffer::~CScreenBuffer()
{

}




CScreenBuffer* CScreenBuffer::GetInstance(void)
{
	return &_CScreenBuffer;
}

//--------------------------------------------
// 콘솔 제어를 위한 준비 작업
// 
//--------------------------------------------
void CScreenBuffer::Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;
	char cCommand[28];

	//-----------------------------------------------------
	// 화면의 커서를 안보이게끔 설정한다.
	//-----------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;				// 커서 크기
	// 이상하게도 0이면 나온다. 1로 하면 안나온다.

//-----------------------------------------------------
// 콘솔화면 (스텐다드 아웃풋) 핸들을 구한다. 
// 
//-----------------------------------------------------
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);


	//-----------------------------------------------------
	// 콘솔화면 크기를 설정한다.
	//col = 가로, lines = 세로
	// 
	//-----------------------------------------------------

	sprintf_s(cCommand, "mode con:cols=%d lines=%d", dfSCREEN_WIDTH, dfSCREEN_HEIGHT + 2);
	system(cCommand);
}

//-----------------------------------------------------
// 콘솔 화면의 커서를 Y, X 좌표로 이동시킨다.
//
//-----------------------------------------------------
void CScreenBuffer::MoveCursor(int iPosY, int iPosX)
{
	COORD stCoord;
	stCoord.Y = iPosY;
	stCoord.X = iPosX;

	//-----------------------------------------------------
	// 원하는 위치로 커서를 이동시킨다.
	//-----------------------------------------------------
	if (SetConsoleCursorPosition(hConsole, stCoord) == 0)
	{
		printf_s("커서 이동 실패했습니다: %d \n", GetLastError());
		return;
	}
}

//-----------------------------------------------------
// 콘솔 화면을 초기화 한다.
// 
//-----------------------------------------------------
void CScreenBuffer::ClearScreen(void)
{
	int iCountX, iCountY;

	//-------------------------------------------------------------
	// 화면 크기만큼 세로, 가로 이중 for 문을 사용하여
	// 각각의 좌표마다 printf(" ");  공백을 전부 출력 해준다.
	//-------------------------------------------------------------
	for (iCountY = 0; iCountY < dfSCREEN_HEIGHT; iCountY++)
	{
		for (iCountX = 0; iCountX < dfSCREEN_WIDTH; iCountX++)
		{
			MoveCursor(iCountY, iCountX);
			printf(" ");
		}
	}
}







//--------------------------------------------------------------------
// 버퍼의 내용을 화면으로 찍어주는 함수.
//
// 적군,아군,총알 등을 szScreenBuffer 에 넣어주고, 
// 1 프레임이 끝나는 마지막에 본 함수를 호출하여 버퍼 -> 화면 으로 그린다.
//--------------------------------------------------------------------
void CScreenBuffer::Flip(void)
{
	int iCnt;
	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; ++iCnt)
	{
		MoveCursor(iCnt, 0);
		printf_s(ScreenBuffer[iCnt]);
	}

	BufferClear();
}

//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void CScreenBuffer::BufferClear(void)
{
	int iCnt;

	memset(ScreenBuffer, ' ', sizeof(ScreenBuffer));

	for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		ScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}


//--------------------------------------------------------------------
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void CScreenBuffer::SpriteDraw(int iY, int iX, char chSprite)
{
	if (iX < 0 || iY < 0 || iX >= dfSCREEN_WIDTH - 1 || iY >= dfSCREEN_HEIGHT)
		return;

	ScreenBuffer[iY][iX] = chSprite;
}
