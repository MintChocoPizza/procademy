// hw6_recursion_graph.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

// map char문자열 이지만 범위는 (0~8 * 0~17)
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

// recursive


// 상 하 좌 우
short dy[] = {-1, 1, 0, 0};
short dx[] = {0, 0, -1, 1};
int num = 0;

void recursive_function(int _y, int _x)
{
	if (Buffer[_y][_x] == true)	// 방문한적이 있다면 그냥 나감
		return;
	Buffer[_y][_x] = true;
	Map[_y][_x] = '.';
	num++;

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
	
	// 상하좌우 순으로 방문한다.
	for (int i = 0; i < 4; i++)
	{
		int ny = _y + dy[i];
		int nx = _x + dx[i];
		
		if (ny < 0 || nx < 0 || 8 < ny || 18 < nx)		// 범위를 벗어나면 넘어감
			continue;
		if (Buffer[ny][nx] == true)					// 방문한 적이 있으면 넘어감
			continue;
		if (Map[ny][nx] != 'O')						// 다음 위치가 'O'이 아니라면 넘어감
			continue;

		recursive_function(ny, nx);
	}
	// 잘 모르겠네
	// 흠...
}

int main()
{
	//시간 측정 시작
	LARGE_INTEGER timer, start, end;
	float DeltaTime;
	QueryPerformanceFrequency(&timer);
	QueryPerformanceCounter(&start);

	for (int i = 0; i < 10; i++)
	{
		memcpy(Map, MapOrign, sizeof(MapOrign));
		memset(Buffer, false, sizeof(Buffer));
		num = 0;
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
		Sleep(50);
		recursive_function(0, 1);
	}





	// 시간 측정 종료
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
