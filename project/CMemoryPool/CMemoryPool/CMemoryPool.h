/*---------------------------------------------------------------

	procademy MemoryPool.

	메모리 풀 클래스 (오브젝트 풀 / 프리리스트)
	특정 데이타(구조체,클래스,변수)를 일정량 할당 후 나눠쓴다.

	- 사용법.

	procademy::CMemoryPool<DATA> MemPool(300, FALSE);
	DATA *pData = MemPool.Alloc();

	pData 사용

	MemPool.Free(pData);


----------------------------------------------------------------*/
#ifndef __CMEMORY_POOL_H__
#define __CMEMORY_POOL_H__

#include <stdio.h>

namespace OreoPizza
{

	template <class DATA>
	struct st_BLOCK_NODE
	{
		DATA data;
		st_BLOCK_NODE* pNext;
	};

	template <class DATA>
	class CMemoryPool
	{

	public:

		//////////////////////////////////////////////////////////////////////////
		// 생성자, 파괴자.
		//
		// Parameters:	(int) 초기 블럭 개수.
		//				(bool) Alloc 시 생성자 / Free 시 파괴자 호출 여부
		// Return:
		//////////////////////////////////////////////////////////////////////////
		CMemoryPool(int iBlockNum, bool bPlacementNew = false);
		virtual	~CMemoryPool();


		//////////////////////////////////////////////////////////////////////////
		// 블럭 하나를 할당받는다.  
		//
		// Parameters: 없음.
		// Return: (DATA *) 데이타 블럭 포인터.
		//////////////////////////////////////////////////////////////////////////
		DATA* Alloc(void);

		//////////////////////////////////////////////////////////////////////////
		// 사용중이던 블럭을 해제한다.
		//
		// Parameters: (DATA *) 블럭 포인터.
		// Return: (BOOL) TRUE, FALSE.
		//////////////////////////////////////////////////////////////////////////
		bool	Free(DATA* pData);


		//////////////////////////////////////////////////////////////////////////
		// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
		//
		// Parameters: 없음.
		// Return: (int) 메모리 풀 내부 전체 개수
		//////////////////////////////////////////////////////////////////////////
		int		GetCapacityCount(void) { return m_iCapacity; }

		//////////////////////////////////////////////////////////////////////////
		// 현재 사용중인 블럭 개수를 얻는다.
		//
		// Parameters: 없음.
		// Return: (int) 사용중인 블럭 개수.
		//////////////////////////////////////////////////////////////////////////
		int		GetUseCount(void) { return m_iUseCount; }

		//////////////////////////////////////////////////////////////////////////
		// 리스트를 순회한다.
		// 
		//////////////////////////////////////////////////////////////////////////
		void	TraverseMemoryPool(void);


		// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리.

		st_BLOCK_NODE<DATA> _pFreeNode;

	private:
		int		m_iCapacity;
		int		m_iUseCount;
		bool	m_bPlacementNew;
	};

	template<class DATA>
	inline CMemoryPool<DATA>::CMemoryPool(int iBlockNum, bool bPlacementNew) : 
		m_iCapacity(iBlockNum), m_iUseCount(0), m_bPlacementNew(bPlacementNew)
	{
		int iCnt;
		_pFreeNode.pNext = NULL;
		for (iCnt = 0; iCnt < iBlockNum; ++iCnt)
		{
			st_BLOCK_NODE<DATA>* pNewNode = (st_BLOCK_NODE<DATA> *)malloc(sizeof(st_BLOCK_NODE<DATA> *));
			if (pNewNode == NULL)
				throw;

			new(&pNewNode->data) DATA();
			
			(*pNewNode).pNext = _pFreeNode.pNext;
			_pFreeNode.pNext = pNewNode;
		}
	}	

	template<class DATA>
	inline CMemoryPool<DATA>::~CMemoryPool()
	{
		
	}

	template<class DATA>
	inline DATA* CMemoryPool<DATA>::Alloc(void)
	{
		st_BLOCK_NODE<DATA>* tempNode = _pFreeNode.pNext;

		_pFreeNode.pNext = tempNode->pNext;

		if (m_bPlacementNew == true)
		{
			new(&(tempNode->data)) DATA();
		}

		return &tempNode->data;
	}

	template<class DATA>
	inline bool CMemoryPool<DATA>::Free(DATA* pData)
	{
		if (m_bPlacementNew == true)
		{
			pData->~DATA();
		}

		// 현재는 data의 위치와 구조체의 주소가 같음.
		st_BLOCK_NODE<DATA>* st_makeStruct = (st_BLOCK_NODE<DATA> *)pData;

		st_makeStruct->pNext = _pFreeNode.pNext;
		_pFreeNode.pNext = st_makeStruct;

		return true;
	}

	template<class DATA>
	inline void CMemoryPool<DATA>::TraverseMemoryPool(void)
	{
		st_BLOCK_NODE<DATA>* curNode = _pFreeNode.pNext;
		int iCnt = 0;

		for (curNode = _pFreeNode.pNext; curNode != NULL; curNode = (*curNode).pNext)
		{
			printf_s("노드 출력: %d \n", iCnt);
			printf_s("노드의 주소: %p, 다음 노드 주소: %p \n", &curNode->data, curNode->pNext);
			++iCnt;
		}
	}

}





















#endif // !__CMEMORY_POOL_H__
