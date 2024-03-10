#ifndef __CLIST_H__
#define __CLIST_H__

//////////////////////////////////////////////////////////////////////////////////////////
// 템플릿 클래스의 경우 정의부와 선언부를 분리할 수 없다. 
// 
// 하나의 파일에 작성해야 함
// 
//////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class CList
{
public:
	struct Node
	{
		T _data;
		Node* _Prev;
		Node* _Next;
	};

	class iterator
	{
	private:
		Node* _node;
	public:
		iterator(Node* node = nullptr)
		{
			// 인자로 들어온 Node 포인터를 저장
			_node = node;
		}

		inline iterator operator ++(int);
		inline iterator& operator ++();
		inline iterator operator --(int);
		inline iterator& operator --();
		inline bool operator !=(const iterator& other);
		inline T& operator *();

	};


private:
	int _size = 0;
	Node _head;
	Node _tail;

public:
	CList();
	~CList();

	// 모든 요소를 순회하면 출력한다.
	void printAll(void);

	//-----------------------------------------------------
	// 4로 초기화된 3개의 원소를 할당한다.
	// 
	// ex)
	// it.assign(3,4);
	// 
	//-----------------------------------------------------
	void assign(int count, T data);

	// 맨 앞의 원소를 반환(return), 참조 한다.
	T& front(void);
	// 맨 뒤의 원소를 반환(return), 참조 한다.
	T& back(void);
	
	//-----------------------------------------------------
	// 맨 앞의 원소를 가리키는 iterator를 반환한다. 
	// 
	// ex) list<int>::iterator iter;
	// iter = lt.begin();
	// 
	//-----------------------------------------------------
	inline iterator begin(void);

	//-----------------------------------------------------
	// 맨 마지막의 원소의 다음을 가리키는 iterator를 반환한다. 
	// 
	// ex) list<int>::iterator iter;
	// iter = It.end();
	// 
	//-----------------------------------------------------
	inline CList::iterator end(void);

	// 앞쪽으로 원소 data를 삽입한다.
	inline void push_front(T data);
	// 뒤쪽으로 원소 data를 삽입한다.
	inline void push_back(T data);
	// 맨 마지막 원소를 제거한다.
	inline void pop_back(void);
	// 맨 첫번째 원소를 제거한다.
	inline void pop_front(void);
	//

};





//////////////////////////////////////////////////////////////////////////////////////////
// iterator 클래스 
// 
// 연산자 오버로딩
// 
//////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename CList<T>::iterator CList<T>::iterator::operator++ (int)
{
	CList<T>::iterator iter(this->_node->_Next);
	this->_node = this->_node->_Next;

	return iter;
}

template <typename T>
inline typename CList<T>::iterator& CList<T>::iterator::operator++ ()
{
	this->_node = this->_node->_Next;
	return *this;
}

template <typename T>
inline typename CList<T>::iterator CList<T>::iterator::operator-- (int)
{
	CList<T>::iterator iter(this->_node->_Prev);
	this->_node = this->_node->_Prev;

	return iter;
}

template <typename T>
inline typename CList<T>::iterator& CList<T>::iterator::operator--()
{
	this->_node = this->_node->_Prev;
	return *this;
}

template<typename T>
inline bool CList<T>::iterator::operator!=(const iterator& other)
{
	if (this->_node == other._node)
		return false;
	else
		return true;
}

template<typename T>
inline T& CList<T>::iterator::operator*()
{
	// TODO: 여기에 return 문을 삽입합니다.

	return this->_node->_data;
}
















//////////////////////////////////////////////////////////////////////////////////////////
// 같은 파일에 정의부 작성
// 
// CList 클래스
//////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------
// 생성자, 소멸자
// 
// 생성자와 소멸자는 클래스가 스택에 할당되어 생성되거나, new 를 사용하여 동적으로 할당할 때
// 자동으로 생성자가 동작한다. 
// 
// 단, malloc, free 를 사용하여 동적 할당을 할 경우 생성자와 소멸자가 동작하지 않는다.
//----------------------------------------------------------------------------------------
template <typename T>
CList<T>::CList()
{
	_head._Prev = NULL;
	_head._Next = &_tail;

	_tail._Next = NULL;
	_tail._Prev = &_head;
}

template <typename T>
CList<T>::~CList()
{

}

//----------------------------------------------------------------------------------------
// 클래스의 함수
// 
//----------------------------------------------------------------------------------------
template <typename T>
void CList<T>::printAll(void)
{
	Node* curNode;

	for (curNode = _head._Next; curNode != &_tail; curNode = curNode->_Next)
	{
		std::cout << curNode->_data << ',';
	}

	std::cout << "\n reverse \n";

	for (curNode = _tail._Prev; curNode != &_head; curNode = curNode->_Prev)
	{
		std::cout << curNode->_data << ',';
	}

	std::cout << std::endl;
	
}

//////////////////////////////////////////////////////////////////////////////////////////
// 이 함수들은 원래 list에서도 인라인으로 선언되어 있다. 
// 
// 정의를 이렇게 작성할 수 있지만 인라인 위치로 옮긴다.
// 
//////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
typename CList<T>::iterator CList<T>::begin(void)
{
	CList<T>::iterator iter(_head._Next);
	return iter;
}

template <typename T>
typename CList<T>::iterator CList<T>::end(void)
{
	CList<T>::iterator iter(&_tail);
	return iter;
}

template <typename T>
inline void CList<T>::push_front(T data)
{
	Node* pNewNode = (Node*)malloc(sizeof(Node));
	if (pNewNode == NULL)
		return;

	pNewNode->_data = data;
	pNewNode->_Next = _head._Next;
	pNewNode->_Prev = &_head;

	_head._Next->_Prev = pNewNode;
	_head._Next = pNewNode;
}

template <typename T>
void CList<T>::push_back(T data)
{
	Node* pNewNode = (Node*)malloc(sizeof(Node));
	if (pNewNode == NULL)
		return;

	pNewNode->_data = data;
	pNewNode->_Next = &_tail;
	pNewNode->_Prev = _tail._Prev;

	_tail._Prev->_Next = pNewNode;
	_tail._Prev = pNewNode;
}

template <typename T>
void CList<T>::pop_back(void)
{
	Node *removeNode = _tail._Prev;
	Node* prevNode = removeNode->_Prev;

	prevNode->_Next = &_tail;
	_tail._Prev = prevNode;
}

template <typename T>
void CList<T>::pop_front(void)
{
	Node* removeNode = _head._Next;
	Node* nextNode = removeNode->_Next;

	nextNode->_Prev = &_head;
	_head._Next = nextNode;
}

#endif // !__CLIST_H__

