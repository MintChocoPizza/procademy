

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

		void insert(int num);


	private:
		st_NODE *st_Root;
	};

	OreoPizza::CTREE::CTREE() : st_Root(nullptr)
	{
	}

	OreoPizza::CTREE::~CTREE()
	{
	}

	inline void CTREE::insert(int num)
	{
		st_NODE* pNewNode = (st_NODE*)malloc(sizeof(st_NODE));

		if (pNewNode == NULL)
			throw;

		pNewNode->key = num;
		pNewNode->pLeft = nullptr;
		pNewNode->pRight = nullptr;

		if (st_Root == nullptr)
		{
			st_Root = pNewNode;
			return;
		}

		

		st_NODE* pCurNode = st_Root;

		while (pCurNode != nullptr)
		{
			if (pCurNode->key < num)
				pCurNode = pCurNode->pRight;
			else
				pCurNode = pCurNode->pLeft;

		}
	}

}

#endif // !__TREE_H__


