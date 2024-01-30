// hw7_bfs_dfs.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

struct SAVEPARENT       // y,x가 NULL이면 시작노드. 0이면 비어있다.
{
    int parentY;
    int parentX;

}saveParent[10][20];

char originalMap[10][20] =
{
    {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' },
    {'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x', 'x', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x' },
    {'x', ' ', 'x', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', 'x', ' ', 'x', 'x', 'x', ' ', 'x', 'x', 'x', 'x', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', ' ', ' ', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', 'x', 'x', 'x', 'x', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x', 'x', ' ', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x', 'x', 'x', ' ', 'x' },
    {'x', 'x', 'x', 'x', 'x', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', ' ' }
};

char Map[10][20] =
{
    {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' },
    {'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x', 'x', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x' },
    {'x', ' ', 'x', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', 'x', ' ', 'x', 'x', 'x', ' ', 'x', 'x', 'x', 'x', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', 'x', ' ', 'x', ' ', ' ', ' ', ' ', ' ', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', 'x', 'x', 'x', 'x', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x', 'x', ' ', 'x', ' ', 'x' },
    {'x', ' ', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x', 'x', 'x', ' ', 'x' },
    {'x', 'x', 'x', 'x', 'x', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', ' ' }
};


// 상하좌우
int dy[] = { -1, 1, 0, 0 };
int dx[] = { 0, 0, -1, 1 };


// 함수 실행횟수 체크
int count = 0;


// 스택 만들기
void* qIn = NULL;   // void * 는 사용할 때 원하는 형으로 형변환 하여 사용하면 된다. 
void* qOut = NULL;  // 상황에 따라 Stack의 top으로 사용한다.




struct Stack
{
    char a; //  1 + 3
    int y;  //  4 + 0
    int x;  //  4 + 4
    int parentY;
    int parentX;
    Stack* prev;    // 8 + 0   
}sNode;

bool IsEmpty()
{
    if (qOut == NULL)
        return true;
    return false;
}

bool sPush(int _y, int _x, int  _pY, int _pX)
{
    Stack* newNode = (Stack*)malloc(sizeof(Stack));
    if (newNode == NULL)
        return false;
    
    // newNode에 데이터 할당 및 아래 노드에 연결한다.
    // (*newNode).y = _y;
    newNode->y = _y;
    newNode->x = _x;
    newNode->parentY = _pY;
    newNode->parentX = _pX;
    newNode->prev = (Stack *)qOut;

    // 새로 연결한 노드를 Top이 가르키게 한다.
    qOut = (void *)newNode;
    
    return true;
}

bool sPop(int* _y, int* _x, int *_pY, int *_pX)
{
    // 1. 삭제할 노드를 temp에 임시로 연결한다. 노드가 비어있다면 false
    Stack* temp = (Stack*)qOut;
    if (temp == NULL)
        return false;

    // 2. 포인터 변수에 반환할 데이터를 넘겨준다. 
    *_y = temp->y;
    *_x = temp->x;
    *_pY = temp->parentY;
    *_pX = temp->parentX;

    // 3. Top의 위치를 아래 node에 연결한다. 
    qOut = temp->prev;

    // 4. 기존 Top를 free 한다.
    free(temp);
    return true;
}

// 굳이 양방향 큐를 만들 이유는 없다. 
struct Queue {
    int y;
    int x;
    int parentY;
    int parentX;
    Queue* prev;
}qNode;

bool qPush(int _y, int _x, int _parentY, int _parentX)
{
    // 새로운 동적 할당을 받는다.
    Queue* newNode = (Queue*)malloc(sizeof(Queue));
    if (newNode == NULL)
        return false;

    // 새로운 node에 데이터를 넣는다.
    newNode->y = _y;
    newNode->x = _x;
    newNode->parentY = _parentY;
    newNode->parentX = _parentX;
    newNode->prev = NULL;


    // qIn이 비어있다면
    if (qIn == NULL)
    {
        // 시작과 끝 노드를 newNode에 연결하면 된다.
        qIn = newNode;
        qOut = newNode;

    }
    else
    {
        ((Queue*)qIn)->prev = newNode;
        qIn = newNode;
    }

    return true;;
}

bool qPop(int* _y, int* _x, int *_parentY, int *_parentX)
{
    if (qIn == NULL)
        return false;

    Queue* temp = (Queue*)qOut;
    *_y = temp->y;
    *_x = temp->x;
    *_parentY = temp->parentY;
    *_parentX = temp->parentX;

    qOut = temp->prev;
    if (qOut == NULL)
        qIn = NULL;

    free(temp);
    return true;
}


void printMap()
{
    int y = 9;
    int x = 19;


    while (y != NULL && x != NULL)
    {
        originalMap[y][x] = '.';
        int tempY = saveParent[y][x].parentY;
        int tempX = saveParent[y][x].parentX;

        y = tempY;
        x = tempX;
    }

    system("cls");
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            printf_s("%c", originalMap[i][j]);
        }
        printf_s("\n");
    }
    printf_s("실행횟수: %d \n", count);
}


// 깊이 우선 탐색
void dfs()
{
    bool isCheck = false;

    // 시작 위치(1,1) 종료 위치(19 , 19)

    // 스택에 시작 노드를 넣는다. 
    sPush(1, 1, NULL, NULL);

    while (IsEmpty() == false)
    {
        int y;
        int x;
        int parentY;
        int parentX;
        sPop(&y, &x, &parentY, &parentX);
        saveParent[y][x].parentY = parentY;
        saveParent[y][x].parentX = parentX;
        count++;

        // 기저사례
        // (19, 19) 라면 종료한다.
        if (y == 9 && x == 19)
        {
            isCheck = true;
            break;
        }
        // 이미 방문한적 있다면 넘어간다.
        if(Map[y][x] != ' ')
            continue;

        Map[y][x] = '.';
        system("cls");
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                printf_s("%c", Map[i][j]);
            }
            printf_s("\n");
        }
        printf_s("실행횟수: %d \n", count);
        Sleep(50);

        for (int i = 0; i < 4; i++)
        {
            int ny = y + dy[i];
            int nx = x + dx[i];

            // 예외처리
            // 1. Map을 벗어난 경우
            if (ny < 0 || nx < 0 || 20 < ny || 20 < nx)
                continue;
            // 2. 지나갈 수 없는경우, ' '이 아닌경우
            if (Map[ny][nx] != ' ')
                continue;
            // 3. 방문한 적이 있는 경우
            if (saveParent[ny][nx].parentY != -1 && saveParent[ny][nx].parentX != -1)
                continue;

            sPush(ny, nx, y, x);
        }
    }

    if (isCheck)
        printMap();
    else
        printf_s("경로를 찾을 수 없습니다.");
}

// 너비 우선 탐색
void bfs()
{
    bool isCheck = false;

    // 시작 위치(1,1) 종료 위치(9 , 19)

    // 스택에 시작 노드를 넣는다. 
    qPush(1, 1, NULL, NULL);

    while (IsEmpty() == false)
    {
        int y;
        int x;
        int parentY;
        int parentX;
        qPop(&y, &x, &parentY, &parentX);
        saveParent[y][x].parentY = parentY;
        saveParent[y][x].parentX = parentX;
        count++;

        // 기저사례
        // (19, 19) 라면 종료한다.
        if (y == 9 && x == 19)
        {
            isCheck = true;
            break;
        }
        // 이미 방문한적 있다면 넘어간다.
        if (Map[y][x] != ' ')
            continue;

        Map[y][x] = '.';
        system("cls");
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                printf_s("%c", Map[i][j]);
            }
            printf_s("\n");
        }
        printf_s("실행횟수: %d \n", count);
        Sleep(50);

        for (int i = 0; i < 4; i++)
        {
            int ny = y + dy[i];
            int nx = x + dx[i];

            // 예외처리
            // 1. Map을 벗어난 경우
            if (ny < 0 || nx < 0 || 20 < ny || 20 < nx)
                continue;
            // 2. 지나갈 수 없는경우, ' '이 아닌경우
            if (Map[ny][nx] != ' ')
                continue;
            // 3. 방문한 적이 있는 경우
            if (saveParent[ny][nx].parentY != -1 && saveParent[ny][nx].parentX != -1)
                continue;

            qPush(ny, nx, y, x);
        }
    }

    if (isCheck)
        printMap();
    else
        printf_s("경로를 찾을 수 없습니다.");
}

int main()
{
    memset(saveParent, -1, sizeof(saveParent));

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            printf_s("%c", originalMap[i][j]);
        }
        printf_s("\n");
    }
    Sleep(50);

    //dfs();


    bfs();
    
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
