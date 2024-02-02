// hw8_hash_table_EX.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#define dfHASH_MAX 10

typedef struct st_HASH_DATA
{
    bool bUse;
    int iKey;
    int iData;
    struct st_HASH_DATA* pNext;
} stData;

stData g_stHash[dfHASH_MAX];

//---------------------------
// Key 값에 대한 해쉬 값 생성
// 0 ~ dfHASH_MAX -1 범위로 나오도록 한다.
//---------------------------

int MakeHash(int iKey)
{
    return iKey % dfHASH_MAX;
}

//---------------------
// 해쉬 테이블에 값을 넣는다.
//----------------------------

bool insert(int iKey, int iData)
{
    stData* pNode;
    stData* pNewNode;
    int iHash = MakeHash(iKey);

    //-------------------------
    // 찾은 자리가 비어있다면 그냥 바로 넣는다.
    //---------------------------------------------------

    if (!g_stHash[iHash].bUse)
    {
        g_stHash[iHash].bUse = true;
        g_stHash[iHash].iKey = iKey;
        g_stHash[iHash].iData = iData;
        g_stHash[iHash].pNext = NULL;
    }
    else
    {
        //----------------------------------------------------------------------------------
        // 찾은 자리가 사용중이면 노드를 새엇ㅇ하여 리스트로 추가한다. 
        // 그런데 이미 노드를 생성하여 두 개 이상을 가지고 있을 수 있으므로
        // pNext 가 NULL값이 나오는 위치 까지 이동하여 추가한다.
        //----------------------------------------------------------------------
        {
            pNode = &g_stHash[iHash];
            // a[100]이 있을 때
            // a == &a[0] 같다. 
            // 고로 &g_stHash[iHash] == 저 위치의 주소를 의미한다.
            // pNode == g_stHash[iHash]의 주소를 가르킨다.
            // pNode->pNext의 의미는 (*pNode).pNext 이다.
            //------------------------------------------
            // pNext가 NULL 값이면 끝이다.
            //---------------------------------------
            while (pNode->pNext != NULL)
            {
                pNode = pNode->pNext;
            }
            //--------------------------------
            // 새로 추가할 노드 데이터 생성
            //---------------------------------
            pNewNode = (stData *)malloc(sizeof(stData));
            // 위 코드는 pNewNode는 stData형의 데이터의 동적 할당 주소를 동적할당 받고
            // 그 동적할당의 크기는 stData만큼이다.
            memset(pNewNode, 0, sizeof(stData));

            pNewNode->bUse = true;
            pNewNode->iKey = iKey;
            pNewNode->iData = iData;
            pNewNode->pNext = NULL;
            //----------------------------------------
            // 위에서 찾은 pNode의 다음 위치에 새로 만든 노드를 연결한다. 
            //---------------------------------------------------
            pNode->pNext = pNewNode;
        }
        printf_s("\n inset success \n");
        return true;
    }
}

//-------------------------------------------------------------------
// 해쉬 테이블에서 값을 찾는다.
//-------------------------------------------------------------------
bool Find(int iKey, int* pDataOut)
{
    stData* pNode;
    int iHash = MakeHash(iKey);

    //-------------------------------------------------------------------
    // 해쉬값의 배열에 접근하여 포인터를 먼저 얻는다.
    //-------------------------------------------------------------------
    pNode = &g_stHash[iHash];

    while (pNode != NULL)
    {
        //-------------------------------------------------------------------
        // 데이터를 찾았다면 종료하고, 찾지 못했다면
        // Next로 계속 들어가면서 찾아본다.
        //-------------------------------------------------------------------

        if (pNode->bUse && pNode->iKey == iKey)
        {
            *pDataOut = pNode->iData;
            return true;
        }
        pNode = pNode->pNext;
    }
    return false;
}

int main()
{
    
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
