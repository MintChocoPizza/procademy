#ifndef __MYNEW_H__
#define __MYNEW_H__

#include <time.h>

struct stALLOCINFO
{
	void* ptr;
	size_t size;
	char fileName[128];
	int line;
	bool arr;
	stALLOCINFO* pPreviousNode;
	stALLOCINFO* pNextNode;
};



class cMYNEW
{
private:
	stALLOCINFO* pHead;
	stALLOCINFO* pTail;

public:
	cMYNEW()
	{
		pHead = NULL;
		pTail = NULL;
	}
	~cMYNEW()
	{
		//------------------------------------------------------------------------
		// 파일에 저장한다. 
		// 
		//------------------------------------------------------------------------
		stALLOCINFO* pCur = pHead;
		while (pCur != NULL)
		{
			saveFile("LEAK", pCur->ptr, pCur);

			pCur = pCur->pNextNode;
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////
	// 굳이 클래스 안에 또 new가 있어야 하나???
	// 있으면 뭐하나????
	// 
	//
	///////////////////////////////////////////////////////////////////////////////////////
	//void* operator new(size_t size)
	//{
	//	//메모리 할당
	//	void* ptr = malloc(size);


	//	return ptr;
	//}
	//void operator delete(void* p)
	//{
	//	// 메모리 해제
	//	free(p);
	//}













	///////////////////////////////////////////////////////////////////////////////////////
	// 파일에 데이터 저장 함수
	// 
	///////////////////////////////////////////////////////////////////////////////////////
	void saveFile(const char* log, void* ptr, stALLOCINFO* stData)
	{
		FILE* pFile;
		errno_t err;
		struct tm newtime;
		__time64_t long_time;
		char timebuf[26];


		_time64(&long_time);
		err = _localtime64_s(&newtime, &long_time);
		if (err)
		{
			printf("Invalid argument to _localtime64_s.");
			return;
		}
		sprintf_s(timebuf, "Alloc_%d%d%d_%d%d%d.txt", newtime.tm_year, newtime.tm_mon, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec);
		

		err = fopen_s(&pFile, timebuf, "a");
		if (err != NULL)
		{
			printf("Invalid argument to fopen_s. ");
			return;
		}
		

		if (stData == NULL)
		{
			fprintf_s(pFile, "%s [%p] \n", log, ptr);
		}
		else
		{
			// 로그 [주소] [데이터크기] 경로 라인
			fprintf_s(pFile, "%s [%p] [%10zd] %s : %d \n", log, ptr, stData->size, stData->fileName, stData->line);
		}
		
		fclose(pFile);
	}







	///////////////////////////////////////////////////////////////////////////////////////
	// 데이터 관리 함수들
	// 
	///////////////////////////////////////////////////////////////////////////////////////
	bool setAllocInfo(void* ptr, size_t size, const char* file, int line, bool arr)
	{
		int iCnt = 0;
		stALLOCINFO* stNewData = (stALLOCINFO*)malloc(sizeof(stALLOCINFO));

		if (stNewData == NULL)
			return false;

		// 할당된 메모리 정보 저장, 초기화
		(*stNewData).ptr = ptr;
		stNewData->size = size;
		while (*file != '\0')
		{
			stNewData->fileName[iCnt] = *file;
			++file;
			++iCnt;
		}
		stNewData->fileName[iCnt] = '\0';
		stNewData->line = line;
		stNewData->pNextNode = NULL;
		stNewData->pPreviousNode = NULL;
		stNewData->arr = arr;

		//------------------------------------------------------------------------
		// linked list에 newData를 연결한다.
		// 
		//------------------------------------------------------------------------
		if (pHead == NULL)	// 첫번째 노드 삽입
		{
			pHead = stNewData;
			pTail = stNewData;
		}
		else				
		{
			// 1. stNewData의 뒤에 pTail이 가르키는 노드를 연결한다.
			stNewData->pPreviousNode = pTail;
			// 2. pTail의 앞에 stNewData를 연결한다.
			pTail->pNextNode = stNewData;
			// 3.pTail이 stNewData를 가르킨다.
			pTail = stNewData;
		}

		return true;
	}

	stALLOCINFO* findAndAllocInfo(void* ptr)
	{
		stALLOCINFO* stCurrentNode = pHead;
		while (stCurrentNode != NULL)
		{
			if ((*stCurrentNode).ptr == ptr)
			{
				break;
			}

			stCurrentNode = stCurrentNode->pNextNode;
		}

		return stCurrentNode;
	}

	void deleteAllocInfo(stALLOCINFO* ptr)
	{
		// 1. 뒤 노드가 앞 노드를 가르키게 한다.
		ptr->pPreviousNode->pNextNode = ptr->pNextNode;
		// 2.  앞 노드가 뒤 노드를 가르키게 한다. 
		ptr->pNextNode->pPreviousNode = ptr->pPreviousNode;
		free(ptr);
	}
}cMyNew;











//------------------------------------------------------------------------
// new 연산자 오버로딩 함수
// 
// 	2번째 인자 file이 const가 아니라면 오류가 발생함
//------------------------------------------------------------------------
void* operator new(size_t size, const char* file, int line)
{
	// 기본 new 연산자로 메모리 할당
	void* ptr = malloc(size);
	//void* ptr = cMyNew.operator new(size);

	// 동적 할당 정보를 저장한다.
	bool check = cMyNew.setAllocInfo(ptr, size, file, line, true);

	// 메모리가 제대로 할당 되었는지 예외 체크
	if (ptr == NULL || check == NULL)
		return NULL;

	// 할당된 메모리 주소 반환
	return ptr;
}
void* operator new[](size_t size, const char* file, int line)
{
	// 기본 new 연산자로 메모리 할당
	void* ptr = malloc(size);
	// void* ptr = cMyNew.operator new(size);

	// 동적 할당 정보를 저장한다.
	bool check = cMyNew.setAllocInfo(ptr, size, file, line, false);

	// 메모리가 제대로 할당 되었는지 예외 체크
	if (ptr == NULL || check == NULL)
		return NULL;

	// 할당된 메모리 주소 반환
	return ptr;
}
//------------------------------------------------------------------------
// delete 연산자 오버로딩 함수
// 
//------------------------------------------------------------------------
void operator delete(void* ptr) 
{
	// list에서 해당 주소가 있는지 찾아본다.
	stALLOCINFO* dataPtr = cMyNew.findAndAllocInfo(ptr);


	// 배열 delete를 했어야 하는 경우
	if (dataPtr!=NULL && dataPtr->arr == true)
	{
		cMyNew.saveFile("ARRAY", ptr, dataPtr);
		return;
	}
	
	if (dataPtr != NULL)
	{
		// list에서 주소의 데이터를 삭제한다.
		cMyNew.deleteAllocInfo(dataPtr);

		// 기본 delete 연산자로 메모리 해제
		free(ptr);
		return;
	}

	// 만약 dataPtr이 NULL이면 잘못된 주소를 해지하려 한다면 파일에 NoAlloc를 작성한다.
	if (dataPtr == NULL)
	{
		cMyNew.saveFile("NOALLOC", ptr, NULL);
		return;
	}

	
}
void operator delete[](void* ptr)
{
	// 기본 delete 연산자로 메모리 해제
	free(ptr);
}


#define  new    new(__FILE__, __LINE__)

#endif // !__MYNEW_H__
