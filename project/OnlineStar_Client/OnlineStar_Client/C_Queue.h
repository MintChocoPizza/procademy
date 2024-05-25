
#ifndef __C_QUEUE_H__
#define __C_QUEUE_H__

#include <stdlib.h>
#include <malloc.h>

namespace OreoPizza
{
	template <class DATA>
	class C_Queue
	{
	public:
		inline C_Queue(int size);
		inline ~C_Queue();

		inline void enqueue(const DATA& data);
		inline void dequeue(void);
		inline DATA back(void);
		inline bool isQueueEmpty(void);
		inline bool isQueueFull(void);

		inline bool find(const DATA& data);

		int _In;
		int _Out;
		int _Size;
		int _Data_Size;
		int _Empty_Size
		DATA* _queue;
	};

	template<class DATA>
	inline C_Queue<DATA>::C_Queue(int size) : _In(0), _Out(0), _Size(size), _Data_Size(0), _Empty_Size(size)
	{
		// _queue = (DATA*)malloc(sizeof(DATA) * size);
		_queue = new DATA[size];
	}

	template<class DATA>
	inline C_Queue<DATA>::~C_Queue()
	{
		// free(_queue);
		delete[] _queue;
	}

	template<class DATA>
	inline void C_Queue<DATA>::enqueue(const DATA& data)
	{
		if (isQueueFull())
			return;

		_queue[_In] = data;
		_In = (_In + 1) % _Size;

		++_Data_Size;
		--_Empty_Size;
	}

	template<class DATA>
	inline void C_Queue<DATA>::dequeue(void)
	{
		if (isQueueEmpty())
			return;
		_Out = (_Out + 1) % _Size;
		
		--_Data_Size;
		++_Empty_Size;
	}

	template<class DATA>
	inline DATA C_Queue<DATA>::back(void)
	{
		return _queue[_Out];
	}

	template<class DATA>
	inline bool C_Queue<DATA>::isQueueEmpty(void)
	{
		return _In == _Out;
	}

	template<class DATA>
	inline bool C_Queue<DATA>::isQueueFull(void)
	{
		return (_In + 1) % _Size == _Out;
	}

	template<class DATA>
	inline bool C_Queue<DATA>::find(const DATA& data)
	{
		int in = _In;
		int out = _Out;
		int size = _Size;

		while (out != in)
		{
			if (_queue[out] == data)
				return true;

			out = (out + 1) % size;
		}

		return false;
	}


	
}
#endif // !__C_QUEUE_H__


