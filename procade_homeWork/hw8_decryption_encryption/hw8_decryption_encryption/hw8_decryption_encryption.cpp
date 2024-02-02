// hw8_encoding_decoding.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <conio.h>

// header == oreo pizza

#define KEY 0xff  // 1111 1111

void decryption(unsigned char* _pMemory, unsigned char _Key, int _iSize)
{
	for (int i = 0; i < _iSize; i++)
	{
		// *(_pMemory + i) ^= _Key;
		*_pMemory ^= _Key;
		_pMemory++;
	}
}

int main()
{
	char fileName[128];
	char headerCheck[10];
	int iFileSize;
	int iReadFileSize;
	FILE* pFile;
	unsigned char* pFileMemory;



	// 파일 오픈
	printf_s("암호화 하거나 복호화 할 파일 이름을 입력 해주세요: ");
	scanf_s("%s", fileName, (unsigned)_countof(fileName));

	errno_t err = fopen_s(&pFile, fileName, "rb");
	if (err != NULL)
	{
		printf_s("파일을 열 수 없습니다. ");
		return -1;
	}


	// 파일을 열어서 메모리에 통으로 로드
	fseek(pFile, 0, SEEK_END);
	iFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	pFileMemory = (unsigned char*)malloc(iFileSize);
	if (pFileMemory == NULL)
	{
		printf_s("메모리 할당 실패");
		fclose(pFile);
		return -1;
	}
	iReadFileSize = fread_s(pFileMemory, iFileSize, iFileSize, 1, pFile);
	if (iReadFileSize != 1)
	{
		printf_s("파일 읽기 실패");
		free(pFileMemory);
		fclose(pFile);
		return -1;
	}
	fclose(pFile);



	// 파일 입력을 위하여 새로 열기
	err = fopen_s(&pFile, fileName, "wb");
	if (err != NULL)
	{
		printf_s("파일 열기에 실패했습니다.");
		return -1;
	}

	// 메모리에 있는 파일의 헤더를 확인한다. 
	memset(headerCheck, 0, 10);
	memcpy(headerCheck, pFileMemory, 9);
	if (strcmp("oreopizza", headerCheck) == 0)		// 복호화를 진행햔다.
	{
		decryption(pFileMemory + 9, KEY, iFileSize - 9);
		fwrite(pFileMemory + 9, iFileSize - 9, 1, pFile);
		printf_s("%s 파일의 복호화가 완료 되었습니다. \n", fileName);

		printf_s("..... 아무키나 누르세요....");
		_getch();
	}
	else					/// 암호화를 진행한다.				
	{
		decryption(pFileMemory, KEY, iFileSize);
		sprintf_s(headerCheck, "%s", "oreopizza");
		fwrite(headerCheck, 9, 1, pFile);
		fwrite(pFileMemory, iFileSize, 1, pFile);

		printf_s("%s 파일의 암호화가 완료 되었습니다. \n", fileName);

		printf_s("..... 아무키나 누르세요....");
		_getch();
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
