#include <stdio.h>
#include <Windows.h>

#include "Console.h"

HANDLE hConsole;

//-------------------------------------------------------------
// 이렇게 씁니다.
//
// #include <stdio.h>
// #include <windows.h>
// #incude "Console.h"
//
// void main(void)
// {
//		cs_Initial();
//
//		cs_MoveCursor(0, 0);	// 커서를 0, 0 위치로
//		printf("abcde");		// 0, 0 위치에 글씨를 찍음
//		cs_MoveCursor(20, 10);	// 커서를 20, 10 위치로
//		printf("abcde");		// 20, 10 위치에 글씨를 찍음
//
//		// 화면을 지울 때
//		cs_ClearScreen();
// }
//-------------------------------------------------------------





//--------------------------------------------
// 콘솔 제어를 위한 준비 작업
// 
//--------------------------------------------
void cs_Initial(void)
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

	sprintf_s(cCommand, "mode con:cols=%d lines=%d", dfSCREEN_WIDTH, dfSCREEN_HEIGHT+2);
	system(cCommand);
}


//-----------------------------------------------------
// 콘솔 화면의 커서를 Y, X 좌표로 이동시킨다.
//
//-----------------------------------------------------
void cs_MoveCursor(int iPosY, int iPosX)
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
void cs_ClearScreen(void)
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
			cs_MoveCursor(iCountY, iCountX);
			printf(" ");
		}
	}
}