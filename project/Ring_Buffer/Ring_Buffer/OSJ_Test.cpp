
#include <time.h>
#include <stdlib.h>

#include "OSJ_Test.h"
#include "C_Ring_Buffer.h"

constexpr int CONSOLE_WIDTH = 237;

C_RING_BUFFER rbTemp;
C_RING_BUFFER rb;
char dequeueBuf[CONSOLE_WIDTH + 2];
char peekBuf[10000 + 2];

char tempBuf[10000 + 2];

void printArray(int width, char* pBuf)
{
	for (int i = 0; i < width; ++i) {
		printf("%c", pBuf[i]);
	}
}

// 큐에서 원하는 바이트 수만큼 추출하여 임의의 버퍼에 집어넣는 함수
void extractBytes(std::queue<char>& myQueue, char* buffer, int bytes)
{
	int index = 0;
	while (bytes > 0 && !myQueue.empty()) {
		buffer[index++] = myQueue.front();
		myQueue.pop();
		--bytes;
	}
}

void OSJ_test()
{
	char testString[] = "@234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*!";
	int len = strlen(testString);
	std::queue<char> q;
	srand((unsigned)time(nullptr));
	while (true)
	{
		memset(peekBuf, 0, sizeof(peekBuf));
		memset(dequeueBuf, 0, sizeof(dequeueBuf));
		memset(tempBuf, 0, sizeof(tempBuf));

		int enqueueSize = 0;
		int dequeueSize = 0;

		// 큐에 문자열을 저장한다. 
		for (int i = 0; i < len; ++i)
		{
			q.push(testString[i]);
		}

		// 현재 Queue의 사이즈
		int qSize = q.size();

		// 현재 링버퍼의 FreeSize
		int rbSize = rb.GetFreeSize();
		if (rbSize == 0)
		{
			// 링버퍼가 가득 찼다면 무조건!!!, 랜덤한 크기만큼 Dequeue를 하고, 출력한다. 
			dequeueSize = rb.Dequeue(dequeueBuf, rand() % rb.GetUseSize() + 1);
			printArray(dequeueSize, dequeueBuf);
			continue;
		}

		// 큐 사이즈와, 링버퍼 FreeSize 중 더 작은 값을 기준으로
		// 랜덤한 값을 생성한다.
		int RandomByteToExtract = rand() % (qSize > rbSize ? rbSize : qSize) + 1;

		int index = 0;
		extractBytes(q, tempBuf, RandomByteToExtract);
		enqueueSize = rb.Enqueue(tempBuf, RandomByteToExtract);
		dequeueSize = rb.Dequeue(dequeueBuf, rand() % RAND_MAX + 1);
		if (dequeueSize > 0)
			printArray(dequeueSize, dequeueBuf);

		if (q.size() > 100000)
		{
			memset(dequeueBuf, 0, sizeof(dequeueBuf));
			dequeueSize = rb.Dequeue(dequeueBuf, rb.GetUseSize());
			printArray(dequeueSize, dequeueBuf);
			while (q.size() > 0)
			{
				putchar(q.front());
				q.pop();
			}
		}
	}
}
