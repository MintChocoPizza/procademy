
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <stack>

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

		void printTree(st_NODE* pCurNode, int space);
		void preorder_traverse(st_NODE* pCurNode);
		void loop_preorder_traverse(void);
		void inorder_traverse(st_NODE* pCurNode);
		void postorder_traverse(st_NODE* pCurNode);

	public:void RightRotateNode(st_NODE*& root, st_NODE *& pCurNode);
	public:void LeftRotateNode(st_NODE*& root, st_NODE*& pCurNode);

	private:
	public:st_NODE* st_Root;
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
		st_NODE* pCurNode;
		st_NODE* pPrntNode;
		st_NODE* pNewNode = (st_NODE*)malloc(sizeof(st_NODE));

		if (pNewNode == NULL)
			throw;

		pNewNode->pLeft = &Nil;
		pNewNode->pRight = &Nil;
		pNewNode->key = data;


		if (st_Root == nullptr)
		{
			pNewNode->pParent = nullptr;
			pNewNode->Color = BLACK;
			st_Root = pNewNode;
			return;
		}



		pNewNode->Color = RED;
		pCurNode = st_Root;
		pPrntNode = nullptr;

		while (1)
		{
			pPrntNode = pCurNode;
			if (data < pPrntNode->key)
			{
				pCurNode = pCurNode->pLeft;
				if (pCurNode == &Nil)
				{
					pNewNode->pParent = pPrntNode;
					pPrntNode->pLeft = pNewNode;
					return;
				}
			}
			else if (data > pPrntNode->key)
			{
				pCurNode = pCurNode->pRight;
				if (pCurNode == &Nil)
				{
					pNewNode->pParent = pPrntNode;
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
		if (Choice == 0)
			printTree(st_Root, 0);
		else if (Choice == 1)
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

		if (st_Root == nullptr)
			return false;

		st_deleteNode = st_Root;
		while ((st_deleteNode != &Nil) && (st_deleteNode->key != key))
		{
			st_Parent = st_deleteNode;
			if (key < st_deleteNode->key) st_deleteNode = st_deleteNode->pLeft;
			else st_deleteNode = st_deleteNode->pRight;
		}

		// 삭제할 노드가 없음
		if (st_deleteNode == &Nil)
			return false;

		// 삭제할 노드가 단말노드인 경우 
		if ((st_deleteNode->pLeft == &Nil) && (st_deleteNode->pRight == &Nil))
		{
			if (st_Parent != nullptr)
			{
				if (st_Parent->pLeft == st_deleteNode) st_Parent->pLeft = &Nil;
				else st_Parent->pRight = &Nil;
			}
			else st_Root = nullptr;
		}

		// 삭제할 노드가 자식 노드를 한 개 가진 경우
		else if ((st_deleteNode->pLeft == &Nil) || (st_deleteNode->pRight == &Nil))
		{
			if (st_deleteNode->pLeft != &Nil) st_Child = st_deleteNode->pLeft;
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
			while (st_succ->pRight != &Nil)
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
		if (pCurNode == &Nil)
			return false;
		if (pCurNode->key == key)
			return true;

		return find_traverse(key, pCurNode->pLeft) || find_traverse(key, pCurNode->pRight);
	}

	inline void C_RED_BLACK_TREE::printTree(st_NODE* pCurNode, int space)
	{
		int COUNT = 10;
		int iCnt;

		if (pCurNode == nullptr)
			return;
		if (pCurNode == &Nil)
			return;

		space += COUNT;

		printTree(pCurNode->pRight, space);
		printf_s("\n");
		for (iCnt = COUNT; iCnt < space; ++iCnt)
		{
			printf_s(" ");
		}
		printf_s("%d\n", pCurNode->key);
		printTree(pCurNode->pLeft, space);
	}

	inline void C_RED_BLACK_TREE::preorder_traverse(st_NODE* pCurNode)
	{
		if (pCurNode != &Nil && pCurNode != nullptr)
		{
			printf_s("%d ", pCurNode->key);
			preorder_traverse(pCurNode->pLeft);
			preorder_traverse(pCurNode->pRight);
		}
	}

	inline void C_RED_BLACK_TREE::loop_preorder_traverse(void)
	{
		st_NODE* pCurNode;
		std::stack<st_NODE* > stack;

		if (st_Root == nullptr)
			return;

		stack.push(st_Root);

		while (!stack.empty())
		{
			pCurNode = stack.top();
			stack.pop();

			// 노드 방문
			printf_s("%d ", pCurNode->key);

			// 오른쪽 자식을 스택에 넣는다. 
			if (pCurNode->pRight != &Nil)
				stack.push(pCurNode->pRight);

			// 왼쪽 자식을 스택에 넣는다.
			if (pCurNode->pLeft != &Nil)
				stack.push(pCurNode->pLeft);
		}
	}

	inline void C_RED_BLACK_TREE::inorder_traverse(st_NODE* pCurNode)
	{
		if (pCurNode != &Nil && pCurNode != nullptr)
		{
			inorder_traverse(pCurNode->pLeft);
			printf_s("%d ", pCurNode->key);
			inorder_traverse(pCurNode->pRight);
		}
	}

	inline void C_RED_BLACK_TREE::postorder_traverse(st_NODE* pCurNode)
	{
		if (pCurNode != &Nil && pCurNode != nullptr)
		{
			postorder_traverse(pCurNode->pLeft);
			postorder_traverse(pCurNode->pRight);
			printf_s("%d ", pCurNode->key);
		}
	}
	inline void C_RED_BLACK_TREE::RightRotateNode(st_NODE*& root, st_NODE *& pCurNode)
	{
		st_NODE* pParentsNode = pCurNode->pParent;
		st_NODE* pLeftChild = pCurNode->pLeft;
		st_NODE* pLRChild = pLeftChild->pRight;

		// 1. 왼쪽 자식의 오른쪽 자식을 현재 노드의 왼쪽 자식으로 연결한다. 
		pCurNode->pLeft = pLRChild;
		
		// 2. 역으로 부모 노드도 연결한다. 
		pLeftChild->pParent = pCurNode;

		// 3. 왼쪽 자식의 부모 연결을 조부모로 바꾼다. 
		pLeftChild->pParent = pParentsNode;

		// 4. 만약 조부모 노드가 nullptr이면 현재 노드는 루트 노드였다. 
		if (pParentsNode == nullptr)
			root = pLeftChild;

		// 5. 루트 노드가 아니고, 현재 노드가 조부모의 왼쪽 노드인 경우
		else if (pCurNode == pParentsNode->pLeft)
			pParentsNode->pLeft = pLeftChild;

		// 6. 현재 노드가 조부모의 오른쪽 노드인 경우
		else
			pParentsNode->pRight = pLeftChild;

		// 7. 현재 노드를 왼쪽 자식의 오른쪽 노드로 연결
		pLeftChild->pRight = pCurNode;

		// 8. 현재 노드의 부모 노드 연결 변경
		pCurNode->pParent = pLeftChild;
	}

	inline void C_RED_BLACK_TREE::LeftRotateNode(st_NODE*& root, st_NODE*& pCurNode)
	{
		st_NODE* pParentsNode = pCurNode->pParent;
		st_NODE* pRightChild = pCurNode->pRight;
		st_NODE* pRLChild = pRightChild->pLeft;

		// 1. 오른쪽 자식의 왼쪽 자식을 현재 노드의 오른쪽 자식으로 연결한다. 
		pCurNode->pRight = pRLChild;

		// 2. 역으로 부모 노드도 연결한다. 
		pRLChild->pParent = pCurNode;

		// 3. 오른쪽 자식의 부모 연결을 조부모로 바꾼다. 
		pRightChild->pParent = pParentsNode;

		// 4. 만약 조부모 노드가 nullptr이면 현재 노드는 루트 노드였다.
		if (pParentsNode == nullptr)
			root = pRightChild;

		// 5. 루트 노드가 아니고, 현재 노드가 조부모의 왼쪽 노드인 경우
		else if (pCurNode == pParentsNode->pLeft)
			pParentsNode->pLeft = pRightChild;

		// 6. 현재 노드가 조부모의 오른쪽 노드인 경우
		else
			pParentsNode->pRight = pRightChild;

		// 7. 현재 노드를 오른쪽 자식의 왼쪽 노드로 연결
		pRightChild->pLeft = pCurNode;

		// 8. 현재 노드의 부모 노드 연결 변경
		pCurNode->pParent = pRightChild;
	}
}





#endif // !__RED_BLACK_TREE_H__

