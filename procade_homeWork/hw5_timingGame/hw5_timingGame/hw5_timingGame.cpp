// hw5_timingGame.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>

int g_Timing[]{ 5,10, 14, 17, 20, 25, 29, 31, 33 };
int curPosition = 0;		// 현재 탐색 위치
char outputString[9][10] = { '\0','\0','\0','\0','\0','\0','\0','\0','\0' };		// 출력할 문자열을 그냥 저장하고 있음

char* strCpy(char* _str, const char* _deli)
{
	size_t i;
	i = 0;

	while (_deli[i] != '\0')
	{
		_str[i] = _deli[i];
		++i;
	}
	_str[i] = '\0';
	return _str;
}

int main()
{
	// 프로그램 시작과 동시에 clock()은 계속 시간이 흘러감

	while (1)
	{
		bool isInput = false;
		clock_t curTime = clock();		// 0.001초 
		long score = 0;					// -1:fail(1000~), 0:아직 연산 안함 1:Bad(750~999), 2:Nogood(500~749), 3:Good(250~499), 4:Great(0~249)


		// 입력부 
		if (_kbhit())
		{
			// 키보드 입력 버퍼의 입력을 지워준다.
			_getch();
			isInput = true;
		}

		// 로직부
		/*	원하는 동작 
		* 1. 특정 시간이 지나면 자동으로 fail로 변경 -> 지속적으로 자동으로 되야함
		* 2. 클릭을 입력했을 때, 시간차이를 계산해서 성적을 매겨야함
		*/
		// 현재 탐색 위치를 확인한다.
		score = ((g_Timing[curPosition]) * 1000 - (curTime));

		if (outputString[curPosition][0] == '\0' && score <= (-1000)) // 방문 한적 없이, 현재 시간이 1초가 더 지났을 경우
		{
			strCpy(outputString[curPosition], "Fail");
			++curPosition;
		}
		else if (outputString[curPosition][0] == '\0' && isInput == true)	// 방문 한적이 없고, 키보드 입력이 있다면
		{
			score = abs(score);

			// 점수에 따라 다른 값을 저장한다. 
			if (score < 250)
			{
				strCpy(outputString[curPosition], "Great");
			}
			else if (score < 500)
			{
				strCpy(outputString[curPosition], "Good");
			}
			else if (score < 750)
			{
				strCpy(outputString[curPosition], "Nogood");
			}
			else if (score < 1000)
			{
				strCpy(outputString[curPosition], "Bad");
			}
			else
			{
				strCpy(outputString[curPosition], "Fail");
			}

			curPosition++;
		}
		

		// 랜더부	
		system("cls");		// 콘솔 초기화
		printf_s("%.3lf Sec \n\n", (double)curTime/1000);
		
		for (int i = 0; i < 9; i++)
		{
			printf_s("%2d Sec : %s \n", g_Timing[i], outputString[i]);
		}
		Sleep(100);		
	}
	return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
