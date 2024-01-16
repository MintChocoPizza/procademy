// makeStrFunc.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int strLen(const char* ch)
{
	int ret = 0;
	while (ch[ret] != '\0')
		ret++;
	return ret;
}

char* strCpy(char* dest, const char* sourse)
{
	while (*sourse != '\0')
	{
		*dest = *sourse;
		dest++;
		sourse++;
	}
	*dest = '\0';
	return dest;
}

int strCmp(const char* ch1, const char* ch2)
{
	int ret = 0;
	while (*ch1 != '\0' && *ch2!='\0')
	{
		ret += *ch1 - *ch2;
		ch1++;
		ch2++;
	}
	return ret;
}

char* strCat(char* ch1, const char* ch2)
{
	while (*ch1 != '\0')
		ch1++;

	while (*ch2 != '\0')
	{
		*ch1 = *ch2;

		ch1++;
		ch2++;
	}
	*ch1 = '\0';

	return ch1;
}

int main()
{
	
	int a = strLen("abcde");
	printf_s("%d \n\n", a);

	char ch1[30] = "hellojhj";
	char ch2[30];
	strCpy(ch2, ch1);
	printf_s("%s \n\n", &ch2);

	char ch3[30] = "world";
	char ch4[30] = "world";
	char ch5[30] = "hellosdo";
	printf_s("ch3 == ch4: %d \n", strCmp(ch3, ch4));
	printf_s("ch4 == ch5: %d \n\n", strCmp(ch4, ch5));

	strCat(ch3, ch4);
	printf_s("ch3 + ch4: %s \n", ch3);
	strCat(ch3, ch5);
	printf_s("ch3 + ch5: %s \n", ch3);


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
