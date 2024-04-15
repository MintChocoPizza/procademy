
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

namespace OreoPizza
{
	enum NODE_COLOR
	{
		BLACK = 0,
		RED = 1
	};
	struct st_NODE
	{
		st_NODE* pParent;
		st_NODE* pLeft;
		st_NODE* pRight;

		NODE_COLOR Color;

		int key;
	};

	class C_RED_BLACK_TREE
	{
	public:
		C_RED_BLACK_TREE();
		~C_RED_BLACK_TREE();

		void insert(int data);
		bool find(const int key);
		void traverse(int Choice);
		bool erase(const int key);

	private:
		bool find_traverse(const int key, st_NODE* pCurNode);

		void preorder_traverse(st_NODE* pCurNode);
		void inorder_traverse(st_NODE* pCurNode);
		void postorder_traverse(st_NODE* pCurNode);

		void RightRotateNode(st_NODE* pCurNode);
		void LeftRotateNode(st_NODE* x, st_NODE* y);

	private:
		st_NODE* st_Root;
		st_NODE Nil;
	};

	OreoPizza::C_RED_BLACK_TREE::C_RED_BLACK_TREE() : st_Root(nullptr), Nil{nullptr, nullptr, nullptr, BLACK}
	{
	}

	OreoPizza::C_RED_BLACK_TREE::~C_RED_BLACK_TREE()
	{
	}

	inline void C_RED_BLACK_TREE::insert(int data)
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
			else if (data > pPrntNode->key)
			{
				pCurNode = pCurNode->pRight;
				if (pCurNode == nullptr)
				{
					pPrntNode->pRight = pNewNode;
					return;
				}
			}
			else if (data == pPrntNode->key) // 중복 제거
				return;
		}
	}

	inline bool C_RED_BLACK_TREE::find(const int key)
	{

		return find_traverse(key, st_Root);
	}

	inline void C_RED_BLACK_TREE::traverse(int Choice)
	{
		if (Choice == 1)
			preorder_traverse(st_Root);
		else if (Choice == 2)
			inorder_traverse(st_Root);
		else if (Choice == 3)
			postorder_traverse(st_Root);
		else
			return;
	}

	inline bool C_RED_BLACK_TREE::erase(const int key)
	{
		st_NODE* st_deleteNode;
		st_NODE* st_Parent = nullptr;
		st_NODE* st_Child;

		st_NODE* st_succParent;
		st_NODE* st_succ;

		st_deleteNode = st_Root;
		while ((st_deleteNode != nullptr) && (st_deleteNode->key != key))
		{
			st_Parent = st_deleteNode;
			if (key < st_deleteNode->key) st_deleteNode = st_deleteNode->pLeft;
			else st_deleteNode = st_deleteNode->pRight;
		}

		// 삭제할 노드가 없음
		if (st_deleteNode == nullptr)
			return false;

		// 삭제할 노드가 단말노드인 경우 
		if ((st_deleteNode->pLeft == nullptr) && (st_deleteNode->pRight == nullptr))
		{
			if (st_Parent != nullptr)
			{
				if (st_Parent->pLeft == st_deleteNode) st_Parent->pLeft = nullptr;
				else st_Parent->pRight = nullptr;
			}
			else st_Root = nullptr;
		}

		// 삭제할 노드가 자식 노드를 한 개 가진 경우
		else if ((st_deleteNode->pLeft == nullptr) || (st_deleteNode->pRight == nullptr))
		{
			if (st_deleteNode->pLeft != nullptr) st_Child = st_deleteNode->pLeft;
			else st_Child = st_deleteNode->pRight;

			if (st_Parent != nullptr)
			{
				if (st_Parent->pLeft == st_deleteNode) st_Parent->pLeft = st_Child;
				else st_Parent->pRight = st_Child;
			}
			else
				st_Root = st_Child;
		}

		// 삭제할 노드가 자식 노드를 두 개 가진 경우
		else
		{
			st_succParent = st_deleteNode;
			st_succ = st_deleteNode->pLeft;

			// 왼쪽 서브트리에서 후계자(제일 큰 값) 찾기
			while (st_succ->pRight != nullptr)
			{
				st_succParent = st_succ;
				st_succ = st_succ->pRight;
			}

			if (st_succParent->pLeft == st_succ) st_succParent->pLeft = st_succ->pLeft;
			else st_succParent->pRight = st_succ->pLeft;

			st_deleteNode->key = st_succ->key;
			st_deleteNode = st_succ;
		}

		free(st_deleteNode);
		return true;
	}

	inline bool C_RED_BLACK_TREE::find_traverse(const int key, st_NODE* pCurNode)
	{
		if (pCurNode == nullptr)
			return false;
		if (pCurNode->key == key)
			return true;

		return find_traverse(key, pCurNode->pLeft) || find_traverse(key, pCurNode->pRight);
	}

	inline void C_RED_BLACK_TREE::preorder_traverse(st_NODE* pCurNode)
	{
		if (pCurNode != nullptr)
		{
			printf_s("%d ", pCurNode->key);
			preorder_traverse(pCurNode->pLeft);
			preorder_traverse(pCurNode->pRight);
		}
	}

	inline void C_RED_BLACK_TREE::inorder_traverse(st_NODE* pCurNode)
	{
		if (pCurNode != nullptr)
		{
			inorder_traverse(pCurNode->pLeft);
			printf_s("%d ", pCurNode->key);
			inorder_traverse(pCurNode->pRight);
		}
	}

	inline void C_RED_BLACK_TREE::postorder_traverse(st_NODE* pCurNode)
	{
		if (pCurNode != nullptr)
		{
			postorder_traverse(pCurNode->pLeft);
			postorder_traverse(pCurNode->pRight);
			printf_s("%d ", pCurNode->key);
		}
	}
	inline void C_RED_BLACK_TREE::RightRotateNode(st_NODE* pCurNode)
	{
		st_NODE* pLeftChild;
		st_NODE* pRightChild;
		st_NODE* pLR_Child;

	}
	inline void C_RED_BLACK_TREE::LeftRotateNode(st_NODE* x, st_NODE* y)
	{

	}
}





#endif // !__RED_BLACK_TREE_H__

