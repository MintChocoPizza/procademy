// hw4_makeDic.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <Windows.h>
#include <locale.h>

// 5개 단어, 영한 2개, 최대 길이
TCHAR dic[5][2][6] = {
	{_T("i"), _T("나")},
	{_T("you"), _T("너")},
	{_T("love"), _T("사랑")},
	{_T("mom"), _T("엄마")},
	{_T("dad"), _T("아빠")}
};

TCHAR* strLwr(TCHAR* _str)
{
	while (*_str != '\0')
	{
		if (_T('A') <= *_str && *_str <= _T('Z'))
		{
			*_str ^= 0x20;
		}
		_str++;
	}

	return _str;
}

int strLen(const TCHAR* _str)
{
	int i = 0;
	while (_str[i] != '\0')
	{
		++i;
	}
	return i;
}

TCHAR* strTok(TCHAR* _str, const TCHAR* _deli)
{
	setlocale(LC_ALL, "KOREAN");
	int i;
	static TCHAR* subStr = NULL;
	int deliLen = strLen(_deli);

	// 첫 실행, 입력 문자열이 존재하고, subStr이 NULL인 경우
	// subStr에 _str 문자열을 가르킨다.
	if (_str != NULL && !subStr)
		subStr = _str;

	// 문장의 끝, 입력받은 문자열이 없고, subStr이 다음 실행할 문자열로 '\0'을 가르킬때
	// 더 이상 실행할 문장이 없다.
	if (_str == NULL && !subStr)
		return NULL;


	// 시작 위치부터 _deli 문자인 경우 검색
	TCHAR* idx = subStr;		// 다음 실행에 시작할 위치를 선정한다.
	while (1)
	{
		for (i = 0; i < deliLen; ++i)
		{
			if (*idx == _deli[i])
			{
				idx++;
				break;
			}
		}

		if (i == deliLen)	// 위의 반복문을 전부 돌리면, 더 이상 시작 위치부터
		{					// deli 문자가 존재하지 않음 => subStr, idx 둘 다 시작 위치에 서있음
			subStr = idx;
			break;
		}
	}

	// _deli 문자를 검사하고 뒤에 '\0'로 문자열이 끝난 경우
	if (*subStr == '\0')
	{
		subStr = NULL;		// 초기화, 복구
		return NULL;
	}

	// 시작위치를 정했으면 문자열 분리 시작
	while (*subStr != '\0')
	{
		for (i = 0; i < deliLen; ++i)
		{
			if (*subStr == _deli[i])	// 분리할 문자열 찾으면 '\0' 저장하여 끊기
			{
				*subStr = '\0';
				break;
			}
		}

		subStr++;
		if (i < deliLen)
		{
			break;
		}
	}
	return idx;
}

TCHAR* strCat(TCHAR* _dest, const TCHAR* _str)
{
	if (_str == NULL)
	{
		return _dest;
	}

	TCHAR* subStr = _dest;
	
	// 문자열을 연결할 위치를 선정한다. 
	while (*subStr != '\0')
		++subStr;

	// subStr이 \0인 위치부터 _str이 \0이 될때까지 복사한다.
	while (*_str != '\0')
	{
		*subStr = *_str;
		subStr++;
		_str++;
	}

	// 나만의 함수 기능, 뒤에 띄어쓰기를 넣는다. 
	*subStr = ' ';
	
	// 널 문자를 넣어 문자열을 완성한다.
	subStr++;
	*subStr = '\0';

	return _dest;
}

int strCmp(const TCHAR* _s1, const TCHAR* _s2)
{
	while (*_s1 != '\0' && *_s2 != '\0')
	{
		if (*_s1 != *_s2)
			return -1;

		_s1++;
		_s2++;
	}

	return 0;
}

int _tmain(int argc, TCHAR * argv[])
{
	TCHAR str[301];
	TCHAR deli[301] = _T(" ,.");
	TCHAR subStr[301] = _T("\0");

	// 1. 문장을 입력 받는다. 
	_tprintf_s(_T("치환하고 싶은 문장을 입력하시오. \n"));
	_tprintf_s(_T("Input: "));
	_getts_s(str);	// 엔터까지 문장 입력 

	// 2. 입력 받은 문장을 소문자로 변경한다. 
	strLwr(str);

	// 3. strtok를 사용하여 문자열을 분리한다. 
	TCHAR *token = strTok(str, deli);
	while (token != NULL)
	{
		int i;

		// 일치하는 문자열을 찾는다.
		for (i = 0; i < 5; ++i)
		{
			if (!strCmp(token, dic[i][0]))
			{
				// subStr에 치환한 문자열을 합친다. 
				strCat(subStr, dic[i][1]);
				break;
			}
		}
		token = strTok(NULL, deli);
	}

	_tprintf_s(_T("Output: %s \n"), subStr);
	
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
