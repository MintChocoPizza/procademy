// hw6_recursion_repeat.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <Windows.h>

char Map[9][18] = {
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
	memset(Buffer, 0, sizeof(Buffer));

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

	push(0, 1);
	while (empty() != true)
	{
		int y;
		int x;
		pop(&y, &x);
		num++;
		Map[y][x] = '.';

		// 상하좌우 방문 
		for (int i = 0; i < 4; i++)
		{
			int ny = y + dy[i];
			int nx = x + dx[i];

			// 방문한 적 있다면 넘어간다?
			if (Buffer[y][x] == true)
				continue;

			// 범위를 벗어나면 넘어감
			if (ny < 0 || nx < 0 || 9 <= ny || 18 <= nx)
				continue;
			
			// '0' 이 아니라면 넘어감 
			if (Map[ny][nx] != 'O')
				continue;

			push(ny, nx);
		}

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
	}
	

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
