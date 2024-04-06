// hw8_hash_table.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define HASHKEY 10

// 해시 테이블 만들기 
// 단순히 숫자를 입력받아 1000범위의 테이블 안에 매핑하는 단순한 해쉬 테이블이다. 

struct NODE
{
    int stKey;
    int stValue;
    NODE* nextNode;
};

struct HASH
{
    int stKey;
    int stValue;
    int count;
    NODE* nextNode; // 포인터가 아닌 그냥 그냥 노드로 더미 노드로 하여 연결하는게 구현하기 더 쉽다. 조건문도 덜 생김
    NODE* lastNode;
};

HASH hashTable[HASHKEY];

bool push(int _key, int _value)
{
    // 키 값을 이용하여 해쉬 값을 만든다.
    int hash = _key % HASHKEY;

    // 해쉬 테이블의 첫번째 노드라면
    if (hashTable[hash].count == 0)
    {
        hashTable[hash].stKey = _key;
        hashTable[hash].stValue = _value;
        ++hashTable[hash].count;
        return true;
    }
    else
    {
        // 해쉬 테이블의 2번째 이상의 노드라면
        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        if (newNode == NULL)
        {
            printf_s("메모리 할당 오류 \n");
            return false;
        }
        newNode->stKey = _key;
        newNode->stValue = _value;
        newNode->nextNode = NULL;
        ++hashTable[hash].count;
        
        // 현재 노드에 newNode를 연결한다. 
        // 첫번째 연결리스트라면
        if (hashTable[hash].nextNode == NULL)
        {
            hashTable[hash].nextNode = newNode;
            hashTable[hash].lastNode = newNode;
            return true;
        }
        else
        {
            // 첫번째 연결리스트가 아니라면
            hashTable[hash].lastNode->nextNode = newNode;
            hashTable[hash].lastNode = newNode;
            return true;
        }
    }


    return true;
}

void printHashTable()
{
    for (int i = 0; i < HASHKEY; ++i)
    {
        printf_s("\n");
        printf_s("hash[%d]: ", i);

        if (hashTable[i].count == 0)
            continue;

        printf_s("  --> [key: %d, value: %d]", hashTable[i].stKey, hashTable[i].stValue);
        NODE* pCurrent = hashTable[i].nextNode;

        while (pCurrent != NULL)
        {
            printf_s("  --> [key: %d, value: %d]", pCurrent->stKey, pCurrent->stValue);
            pCurrent = pCurrent->nextNode;
        }
    }
}

int main()
{

    memset(hashTable, 0, sizeof(hashTable));
    int iKey;
    int iValue;
    int iMenu;
    while (1)
    {
        system("cls");
        printf_s("1. 해쉬값 넣기 \n");
        printf_s("2. 해쉬값 보기 \n");
        scanf_s("%d", &iMenu);

        switch (iMenu)
        {
        case 1:
            printf_s("해시 테이블에 매핑하고 싶은 키와 값을 입력하세요: ");
            scanf_s("%d %d", &iKey, &iValue);
            if (push(iKey, iValue))
            {
                printf_s("해쉬테이블에 넣었습니다. \n .........아무키나 입력........ \n");
                _getch();
            }
            break;
        case 2:
            printHashTable();
            printf_s("...... 아무키나 입력...... \n");
            _getch();
            break;
        default:
            break;
        }
    }




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
