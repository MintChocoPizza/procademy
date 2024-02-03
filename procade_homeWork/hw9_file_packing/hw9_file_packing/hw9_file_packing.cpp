#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define MAX_FILE_NAME 10
#define OP 0x6f70

#pragma pack(push, 1)
struct st_PACKING_CORE_HEADER
{
	short core;
	DWORD fileNumber;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct st_PACKING_INFO_HEADER
{
	char fileName[64];
	size_t fileSize;
	size_t offset;
};
#pragma pack(pop)

bool packing(char _cFilesName[][64], int _iNum)
{
	errno_t err;
	FILE* pFile;
	long lFileSize;
	unsigned char* pFileMemory;
	size_t llReadFileSize;
	size_t currentOffset = 0;



	//=======================================================
	// CORE_HEADER 구조체를 초기화한다.
	// 시작 파일 갯수는 0이다.
	//=======================================================
	st_PACKING_CORE_HEADER stCoreHeader;
	stCoreHeader.core = OP;
	stCoreHeader.fileNumber = 0;

	//=======================================================
	// INFO_HEADER 구조체를 동적할당하여 생성한다. 
	// 헤더 파일에 한번에 쓰기 위하여,
	// 동적할당 메모리에 데이터를 전부 저장한 후 한번에 write한다.
	// 
	// 크기는 st_PACKING_INFO_HEADER 구조체가  _iNum개
	//=======================================================
	st_PACKING_INFO_HEADER* pInfoHeader = (st_PACKING_INFO_HEADER *)malloc(sizeof(st_PACKING_INFO_HEADER) * _iNum);

	
	//=======================================================
	// 패킹할 파일 읽기 모드로 오픈하기
	//=======================================================
	for (int i = 0; i < _iNum; ++i)
	{
		err = fopen_s(&pFile, _cFilesName[i], "rb");
		if (err != NULL)
		{
			printf_s("%s 파일 열기 실패 \n", _cFilesName[i]);
			return false;
		}

		//=======================================================
		// 오픈한 파일에서 
		// 파일의 크기 알아내기
		//=======================================================
		fseek(pFile, 0, SEEK_END);
		lFileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		
		//=======================================================
		// 오픈한 파일을 패킹 파일에 쓰기 위하여 
		// lFileSize 크기의 메모리를 동적할당하여 저장한다.
		//=======================================================
		pFileMemory = (unsigned char*)malloc(lFileSize);
		llReadFileSize = fread_s(pFileMemory, lFileSize, 1, lFileSize, pFile);
		fclose(pFile);

		//=======================================================
		// "packingFile" 파일을 추가 쓰기로 열어서
		//  pFileMemory에 있는 데이터를
		// lFileSize크기만큼 쓴다.
		//=======================================================
		err = fopen_s(&pFile, "packingFile", "ab");
		if (err != NULL)
		{
			printf_s("packingFile 파일 열기 실패 \n");
			return false;
		}
		llReadFileSize = fwrite(pFileMemory, 1, lFileSize, pFile);
		if (llReadFileSize != lFileSize)
		{
			printf_s("데이터를 전부 쓰지 못했습니다.\n");
			fclose(pFile);
			return false;
		}
		fclose(pFile);


		//=======================================================
		// packingHeader 에 저장할 내용을 업데이트 한다. 
		// 파일 갯수, 파일 이름, 파일 사이즈 ,파일 offset
		//=======================================================
		++stCoreHeader.fileNumber;
		strcpy_s(pInfoHeader[i].fileName, _cFilesName[i]);
		pInfoHeader[i].fileSize = llReadFileSize;
		pInfoHeader[i].offset = currentOffset;
		currentOffset += llReadFileSize;
	}


	//=======================================================
	// packingHeader에 지금까지 저장한 내용을 저장한다
	//=======================================================
	err = fopen_s(&pFile, "packingHeader", "wb");
	if (err != NULL)
	{
		printf_s("packingHeader 파일 열기 실패 \n");

		//=======================================================
		// 위에서 저장한 packingFile 을 삭제한다.
		//=======================================================
		if (remove("packingFile") == NULL)
		{
			printf_s("packingFile 파일을 삭제 완료했습니다. \n");
		}
		else
		{
			printf_s("packingFile 파일을 삭제 실패했습니다. \n");
		}

		return false;
	}
	//=======================================================
	// st_PACKING_CORE_HEADER 를 저장한다.
	//=======================================================
	llReadFileSize = fwrite(&stCoreHeader, 1, sizeof(st_PACKING_CORE_HEADER), pFile);
	if(llReadFileSize != sizeof(st_PACKING_CORE_HEADER))
	{
		printf_s("데이터를 전부 쓰지 못했습니다.\n");
		fclose(pFile);
		//=======================================================
		// 위에서 저장한 packingFile 을 삭제한다.
		//=======================================================
		if (remove("packingFile") == NULL)
		{
			printf_s("packingFile 파일을 삭제 완료했습니다. \n");
		}
		else
		{
			printf_s("packingFile 파일을 삭제 실패했습니다. \n");
		}

		//=======================================================
		// 위에서 저장한 packingHeader 을 삭제한다.
		// fclose 했기 때문에 저장된다.
		//=======================================================
		if (remove("packingHeader") == NULL)
		{
			printf_s("packingHeader 파일을 삭제 완료했습니다. \n");
		}
		else
		{
			printf_s("packingHeader 파일을 삭제 실패했습니다. \n");
		}

		return false;
	}

	//=======================================================
	// st_PACKING_INFO_HEADER 를 저장한다.
	// pInfoHeader 의 크기는 
	// st_PACKING_INFO_HEADER 구조체가 _iNum개 있는 크기이다.
	//=======================================================
	llReadFileSize = fwrite(pInfoHeader, 1, sizeof(st_PACKING_INFO_HEADER)* _iNum, pFile);
	if (llReadFileSize != sizeof(st_PACKING_INFO_HEADER) * _iNum)
	{
		printf_s("데이터를 전부 쓰지 못했습니다.\n");
		fclose(pFile);
		//=======================================================
		// 위에서 저장한 packingFile 을 삭제한다.
		//=======================================================
		if (remove("packingFile") == NULL)
		{
			printf_s("packingFile 파일을 삭제 완료했습니다. \n");
		}
		else
		{
			printf_s("packingFile 파일을 삭제 실패했습니다. \n");
		}

		//=======================================================
		// 위에서 저장한 packingHeader 을 삭제한다.
		// fclose 했기 때문에 저장된다.
		//=======================================================
		if (remove("packingHeader") == NULL)
		{
			printf_s("packingHeader 파일을 삭제 완료했습니다. \n");
		}
		else
		{
			printf_s("packingHeader 파일을 삭제 실패했습니다. \n");
		}

		return false;
	}
	fclose(pFile);


	return true;
}

bool unpacking()
{
	errno_t err;
	FILE* pFile;
	size_t llReadFileSize;
	long lFileSize;
	unsigned char* pFileMemory;
	
	

	
	//=======================================================
	// packingHeader 파일에 패킹에 대한 정보가 들어있다. 
	// st_PACKING_CORE_HEADER,  st_PACKING_INFO_HEADER 
	// 구조체에 정보를 저장한다.
	//=======================================================
	st_PACKING_CORE_HEADER stCoreHeader;
	err = fopen_s(&pFile, "packingHeader", "rb");
	if (err != NULL)
	{
		printf_s("packingHeader 파일 열기 실패 \n");
		return false;
	}
	llReadFileSize = fread_s(&stCoreHeader, sizeof(st_PACKING_CORE_HEADER), 1, sizeof(st_PACKING_CORE_HEADER), pFile);
	if (llReadFileSize != sizeof(st_PACKING_CORE_HEADER))
	{
		printf_s("데이터를 전부 읽지 못했습니다.\n");
		fclose(pFile);
		return false;
	}
	//=======================================================
	// st_PACKING_CORE_HEADER 
	// 구조체에 쓰여있는 파일의 갯수를 기준으로 
	// st_PACKING_INFO_HEADER 
	// 구조체를 파일의 갯수만큼 메모리를 할당한다.
	// 구조체에 정보를 저장한다.
	//=======================================================
	st_PACKING_INFO_HEADER* pInfoHeader = (st_PACKING_INFO_HEADER*)malloc(sizeof(st_PACKING_INFO_HEADER) * stCoreHeader.fileNumber);
	if (pInfoHeader == NULL)
	{
		printf_s("메모리 할당 실패 \n");
		fclose(pFile);
		return false;
	}
	llReadFileSize = fread_s(pInfoHeader, sizeof(st_PACKING_INFO_HEADER) * stCoreHeader.fileNumber, 1, sizeof(st_PACKING_INFO_HEADER) * stCoreHeader.fileNumber, pFile);
	if (llReadFileSize != sizeof(st_PACKING_INFO_HEADER) * stCoreHeader.fileNumber)
	{
		printf_s("데이터를 전부 읽지 못했습니다.\n");
		fclose(pFile);
		return false;
	}
	fclose(pFile);



	//=======================================================
	// 동적으로 할당한 메모리에
	// packingFile 데이터를 저장한다.
	//=======================================================
	err = fopen_s(&pFile, "packingFile", "rb");
	if (err != NULL)
	{
		printf_s("packingFile 파일 열기 실패 \n");
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	lFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	pFileMemory = (unsigned char *)malloc(lFileSize);
	if (pFileMemory == NULL)
	{
		printf_s("메모리 할당 실패 \n");
		fclose(pFile);
		return false;
	}
	llReadFileSize = fread_s(pFileMemory, lFileSize, 1, lFileSize, pFile);
	if (llReadFileSize != lFileSize)
	{
		printf_s("데이터를 전부 읽지 못했습니다.\n");
		fclose(pFile);
		return false;
	}
	fclose(pFile);





	//=======================================================
	// fileNumber 만큼 돌면서 파일을 생성한다.
	//=======================================================
	for (int i = 0; i < stCoreHeader.fileNumber; ++i)
	{
		//=======================================================
		// pInfoHeader 구조체의 정보를 토대로 파일을 생성한다.
		//=======================================================
		err = fopen_s(&pFile, pInfoHeader[i].fileName, "wb");
		if (err != NULL)
		{
			printf_s("%s 파일 열기 실패 \n", pInfoHeader[i].fileName);
			return false;
		}

		llReadFileSize = fwrite((pFileMemory + pInfoHeader[i].offset), 1, pInfoHeader[i].fileSize, pFile);
		if (llReadFileSize != pInfoHeader[i].fileSize)
		{
			printf_s("데이터를 전부 쓰지 못했습니다.\n");
			fclose(pFile);
			return false;
		}
		
		fclose(pFile);
	}


	//=======================================================
	// 패킹 파일들을 삭제한다.
	//=======================================================
	if (remove("packingFile") == NULL)
	{
		printf_s("packingFile 파일을 삭제 완료했습니다. \n");
	}
	else
	{
		printf_s("packingFile 파일을 삭제 실패했습니다. \n");
	}
	if (remove("packingHeader") == NULL)
	{
		printf_s("packingHeader 파일을 삭제 완료했습니다. \n");
	}
	else
	{
		printf_s("packingHeader 파일을 삭제 실패했습니다. \n");
	}



	return true;
}

int main()
{
	int iSwitchOn;
	int iNum;

	char cFilesName[MAX_FILE_NAME][64];

	while (1)
	{
		system("cls");
		printf_s("1. 패킹하기 \n");
		printf_s("2. 패킹풀기 \n");
		scanf_s("%d", &iSwitchOn);

		switch (iSwitchOn)
		{
		case 1:
			printf_s("패킹하고 싶은 파일의 갯수: ");
			scanf_s("%d", &iNum);
			
			//=======================================================
			// 패킹 파일 이름을 저장한다.
			//=======================================================
			for (int i = 0; i < iNum; ++i)
			{
				printf_s("%d번 파일 이름: ", i + 1);
				scanf_s("%s", cFilesName[i], (unsigned)_countof(cFilesName[i]));
			}


			//=======================================================
			// 파일 이름들, 파일 숫자를 인자로 넘겨 패킹한다.
			//=======================================================
			if (packing(cFilesName, iNum))
			{
				printf_s("패킹에 성공하였습니다. \n");
				printf_s("........... 아무키나 입력하세요.......... \n");
				_getch();
			}
			else
			{
				printf_s("패킹에 실패하였습니다. \n");
				printf_s("........... 아무키나 입력하세요.......... \n");
				_getch();
			}
			break;
		case 2:
			if (unpacking())
			{
				printf_s("언패킹에 성공하였습니다. \n");
				printf_s("........... 아무키나 입력하세요.......... \n");
				_getch();
			}
			else
			{
				printf_s("언패킹에 실패하였습니다. \n");
				printf_s("........... 아무키나 입력하세요.......... \n");
				_getch();
			}
			break;
		default:
			break;
		}
	}
	return 0;
}