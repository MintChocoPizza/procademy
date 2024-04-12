

#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <malloc.h>

namespace OreoPizza
{
	struct st_NODE
	{
		int key;
		
		st_NODE* pLeft;
		st_NODE* pRight;
	};

	class CTREE
	{
	public:
		CTREE();
		~CTREE();

		void insert(int data);
		int erase(const int key);

	private:
		st_NODE *st_Root;
	};

	OreoPizza::CTREE::CTREE() : st_Root(nullptr)
	{
	}

	OreoPizza::CTREE::~CTREE()
	{
	}

	inline void CTREE::insert(int data)
	{
		st_NODE* pNewNode = (st_NODE*)malloc(sizeof(st_NODE));

		if (pNewNode == NULL)
			throw;

		pNewNode->key = data;
		pNewNode->pLeft = nullptr;
		pNewNode->pRight = nullptr;

		if (st_Root == nullptr)
		{
			st_Root = pNewNode;
			return;
		}

		

		st_NODE* pCurNode = st_Root;
		st_NODE* pPrntNode = nullptr;

		while (1)
		{
			pPrntNode = pCurNode;
			if (data < pPrntNode->key)
			{
				pCurNode = pCurNode->pLeft;
				if (pCurNode == nullptr)
				{
					pPrntNode->pLeft = pNewNode;
					return;
				}
			}
			else
			{
				pCurNode = pCurNode->pRight;
				if (pCurNode == nullptr)
				{
					pPrntNode->pRight = pNewNode;
					return;
				}
			}
		}
	}

	inline int CTREE::erase(const int key)
	{
		int iEraseCnt = 0;

		if (st_Root == nullptr)
			return 0;

		

		return iEraseCnt;
	}

}

#endif // !__TREE_H__


