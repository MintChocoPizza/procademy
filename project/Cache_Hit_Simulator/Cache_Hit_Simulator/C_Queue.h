
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

		inline void enqueue(DATA* data);
		inline void dequeue(void);
		inline bool isQueueEmpty(void);
		inline bool isQueueFull(void);

		inline bool find(DATA* data);

		int _In;
		int _Out;
		int _Size;
		DATA* queue;
	};

	template<class DATA>
	inline C_Queue<DATA>::C_Queue(int size) : _In(0), _Out(0), _Size(size)
	{
		queue = (DATA*)malloc(sizeof(DATA) * size);
	}

	template<class DATA>
	inline C_Queue<DATA>::~C_Queue()
	{
		free(queue);
	}

	template<class DATA>
	inline void C_Queue<DATA>::enqueue(DATA* data)
	{
		if (isQueueFull())
			return;

		queue[_In] = *data;
		_In = (_In + 1) % _Size;
	}

	template<class DATA>
	inline void C_Queue<DATA>::dequeue(void)
	{
		if (isQueueEmpty())
			return;
		_Out = (_Out + 1) % _Size;
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
	inline bool C_Queue<DATA>::find(DATA* data)
	{
		int in = _In;
		int out = _Out;
		int size = _Size;

		while (out != in)
		{
			if (queue[out] == *data)
				return true;

			out = (out + 1) % size;
		}

		return false;
	}


	
}
#endif // !__C_QUEUE_H__


