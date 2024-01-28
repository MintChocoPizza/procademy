// hw6_recursion_repeat.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <Windows.h>

char Map[9][18];		// 카피할 저장 장소
char MapOrign[9][18] = { // 원본 데이터
	{' ','O','O',' ',' ',' ',' ',' ','O','O','O',' ',' ',' ',' ',' ','O','O'},		// 7
	{' ','O','O','O','O','O','O','O','O','O','O','O','O',' ',' ','O','O','O'},		// 15
	{'O','O','O','O','O',' ',' ','O','O','O','O','O','O','O','O','O','O','O'},		// 16
	{'O','O','O','O',' ',' ',' ','O','O','O','O','O','O','O','O','O','O','O'},		// 15
	{'O','O','O',' ','O',' ',' ','O','O','O','O','O','O','O','O','O','O','O'},		// 15
	{'O','O',' ',' ','O',' ','O','O','O','O','O','O','O','O','O','O','O',' '},		// 14
	{'O','O',' ','O','O',' ','O','O','O','O','O','O','O','O','O',' ',' ',' '},		// 13
	{' ','O',' ',' ','O',' ','O','O','O','O','O','O','O','O',' ',' ',' ',' '},		// 10
	{' ',' ',' ',' ',' ',' ','O','O','O','O','O','O',' ',' ',' ',' ',' ',' '},		// 6
};
bool Buffer[9][19];

// 상 하 좌 우
short dy[] = { -1, 1, 0, 0 };
short dx[] = { 0, 0, -1, 1 };

int num = 0;

struct Node {
	int y;
	int x;
	Node* prev;
}node;

Node* top = NULL;

bool push(int _y, int _x)
{
	Node* newNode = (Node*)malloc(sizeof(Node));

	if (newNode == NULL)
		return false;

	newNode->y = _y;
	newNode->x = _x;

	newNode->prev = top;
	top = newNode;
	return true;
}

bool pop(int *_y, int *_x)
{
	Node* temp = top;
	if (temp == NULL)
		return false;

	*_y = temp->y;
	*_x = temp->x;
	
	top = temp->prev;
	free(temp);
	return true;
}

bool empty()
{
	if (top == NULL)
		return true;
	return false;
}

int main()
{
	//시간 측정 시작
	LARGE_INTEGER timer, start, end;
	float DeltaTime;
	QueryPerformanceFrequency(&timer);
	QueryPerformanceCounter(&start);

	// 출력부
	for(int aaa=0; aaa<10; aaa++)
	{
		// 초기화
		memcpy(Map, MapOrign, sizeof(MapOrign));
		memset(Buffer, false, sizeof(Buffer));
		num = 0;

		system("cls");
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 18; x++)
			{
				printf_s("%c ", Map[y][x]);
			}
			printf_s("\n");
		}
		printf_s("\n 실행횟수: %d \n", num);
		Sleep(50);


		// 처음 시작 위치 지정
		push(0, 1);
		while (empty() != true)
		{
			int y;
			int x;
			pop(&y, &x);

			if (Buffer[y][x] == true)	// 방문한 적이 있다면 나간다.
				continue;

			num++;
			Map[y][x] = '.';
			Buffer[y][x] = true;

			// 출력부
			system("cls");
			for (int y = 0; y < 9; y++)
			{
				for (int x = 0; x < 18; x++)
				{
					printf_s("%c ", Map[y][x]);
				}
				printf_s("\n");
			}
			printf_s("\n 실행횟수: %d \n", num);
			Sleep(50);

			// 상하좌우 방문 
			for (int i = 0; i < 4; i++)
			{
				int ny = y + dy[i];
				int nx = x + dx[i];


				// 범위를 벗어나면 넘어감
				if (ny < 0 || nx < 0 || 8 < ny || 18 < nx)
					continue;
				// 방문한 적 있다면 넘어간다?
				if (Buffer[ny][nx] == true)
					continue;
				// '0' 이 아니라면 넘어감 
				if (Map[ny][nx] != 'O')
					continue;

				push(ny, nx);
			}
		}
	}
	QueryPerformanceCounter(&end);

	printf_s("걸린 시간: %lf \n", (end.QuadPart - start.QuadPart) / (float)timer.QuadPart);
	return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
