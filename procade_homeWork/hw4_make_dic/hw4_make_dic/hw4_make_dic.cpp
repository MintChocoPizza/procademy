// hw4_make_dic.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 언어 5개/ 영어한글 2개 / 최대 30글자 단어
char dic[5][2][30] = {
	{"love", "사랑"},
	{"i", "나"},
	{"you", "너"},
	{"hhkb","해피해킹 키보드"},
	{"mom","엄마"}
};

char* toSmall(char* str)
{
	while (*str != '\0')
	{
		if ('A' <= *str && *str <= 'Z')
			*str ^= 0x20;

		str++;
	}
}

int strLen(const char* str)
{
	int cnt = 0;
	while (str[cnt] != '\0')
		++cnt;
	return cnt;
}

char* makeStrTok(char* _string, const char* _deli)
{
	int i;
	int deliLen = strLen(_deli);	// 몇개 있는지 알아야 for문으로 검사 가능
	char* idx = NULL;

	static char* temp = NULL;		// 임시값 기억 저장소

	if (_string != NULL && !temp)	// 문자열 입력 받고, temp에 값 없을때
		temp = _string;

	if (_string == NULL && !temp)	// 문자열이 끝났을 경우를 먼저 고려
		return NULL;

	idx = temp;
	while (1)	// 연속된 구분자일 경우 대비
	{
		for (i = 0; i < deliLen; ++i)
		{
			if (*idx == _deli[i])
			{
				idx++;
				break;
			}
		}

		if (i == deliLen) // 연속된 구분자 없는 경우
		{
			temp = idx;			// 반복으로 인해 연속된 구분자가 제거된 idx를 temp에 저장
			break;
		}
	}

	if (*temp == '\0')	// 위 과정 후 아무것도 안남았으때를 대비해서 삽입
	{
		temp == NULL;
		return temp;
	}

	// 만약 해당되는 구분자를 찾으면 null로 변환 후 탈출, idx 호출
	while (*temp != '\0')
	{
		for (i = 0; i < deliLen; ++i)
		{
			if (*temp == _deli[i])
			{
				*temp = '\0';	// 여기서 구분자를 null로 변환 &idx는 temp와 같기
								// 때문에 여기서 idx가 토근화된다.(전역변수라 가능)
				break;
			}
		}

		temp++;
		if (i < deliLen)
		{
			break;
		}
	}

	return idx;
	
}


int main()
{
	char cString[151];
	char cStringTemp[151];

	// 입력받기
	printf_s("치환하고 싶은 문장을 입력하세요. \n Input: ");
	gets_s(cString);

	toSmall(cString);

	// 단어 단위 뜯어내기
	char* temp = makeStrTok(cString, " ");
	while (temp != NULL)
	{
		
	}
	


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
