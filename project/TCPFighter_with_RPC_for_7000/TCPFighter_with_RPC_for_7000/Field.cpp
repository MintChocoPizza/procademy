
#include <Windows.h>
#include <list>
#include <unordered_map>
#include "C_Ring_Buffer.h"
#include "main.h"
#include "SerializeBuffer.h"
#include "Session.h"
#include "Define.h"
#include "Field.h"
#include "Player.h"

C_Field C_Field::_C_Field;

// 현재위치 + 8방
// 현재위치, 좌측 상단부터 우측 하단으로 순서대로
int dY[] = { 0, -1,-1,-1,0,0,0,1,1,1 };
int dX[] = { 0, -1,0,1, -1,1, -1,0,1 };

C_Field* C_Field::GetInstance(void)
{
    return &_C_Field;
}

void C_Field::GetSectorAround(int iSectorX, int iSectorY, st_SECTOR_AROUND* pSectorAound)
{
    
}

C_Field::C_Field()
{
    int iCntX;
    int iCntY;
    int Sector_Max_X;
    int Sector_Max_Y;

    _Sector_Max_Y = Sector_Max_Y = dfRANGE_MOVE_BOTTOM / Grid_Y_Size;
    _Sector_Max_X = Sector_Max_X = dfRANGE_MOVE_RIGHT / Grid_X_Size;


    // 3차원 배열: 면, 행, 열 순서
    // 2차원 배열: 행, 열 순서
    // 1차원 배열: 열 순서
    _Sector = new std::list<st_Player*>**[Sector_Max_Y];
    for (iCntY = 0; iCntY < Sector_Max_Y; ++iCntY)
    {
        _Sector[iCntY] = new std::list<st_Player*>*[Sector_Max_X];
        for (iCntX = 0; iCntX < Sector_Max_X; ++iCntX)
        {
            _Sector[iCntY][iCntX] = new std::list<st_Player*>();
        }
    }
}

C_Field::~C_Field()
{
    int iCntY;
    int iCntX;
    int Sector_Max_X;
    int Sector_Max_Y;

    Sector_Max_Y = _Sector_Max_Y;
    Sector_Max_X = _Sector_Max_X;

    for (iCntY = 0; iCntY < _Sector_Max_Y; ++iCntY)
    {
        for (iCntX = 0; iCntX < _Sector_Max_X; ++iCntX)
        {
            // 동적으로 할당된 st_Player들은 Player 클래스에서 어련히 잘 알아서 메모리를 해지했다고 가정한다. 
            // 각 std::list<st_Player*> 해제
            delete _Sector[iCntY][iCntX];
        }
        // 열 배열 해제
        delete[] _Sector[iCntY];
    }
    // 행 배열 해제
    delete[] _Sector;
}



