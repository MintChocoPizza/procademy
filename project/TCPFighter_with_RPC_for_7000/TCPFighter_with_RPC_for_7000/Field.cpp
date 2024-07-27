
#include <Windows.h>
#include <list>
#include <unordered_map>
#include "Protocol.h"
#include "C_Ring_Buffer.h"
#include "SerializeBuffer.h"
#include "main.h"
#include "Session.h"
#include "Define.h"
#include "Field.h"
#include "Player.h"

C_Field C_Field::_C_Field;

//--------------------------------------------
// 현재위치 + 8방
// 현재위치, 좌측 상단부터 우측 하단으로 순서대로
//--------------------------------------------
int dY[] = { 0, -1,-1,-1,0,0,0,1,1,1 };
int dX[] = { 0, -1,0,1, -1,1, -1,0,1 };




st_SECTOR_POS::st_SECTOR_POS() 
{ 
    iX = NULL;
    iY = NULL;
}
st_SECTOR_POS::st_SECTOR_POS(int X, int Y)
{
    iX = X / C_Field::GetInstance()->Grid_X_Size;
    iY = Y / C_Field::GetInstance()->Grid_Y_Size;
}
st_SECTOR_AROUND::st_SECTOR_AROUND() { iCount = 0; }

C_Field* C_Field::GetInstance(void)
{
    return &_C_Field;
}

void C_Field::GetSectorAround(int iSectorX, int iSectorY, st_SECTOR_AROUND* pSectorAound)
{
    int iCnt;
    int nY;
    int nX; 
    int iCount;

    iCount = 0;

    for (iCnt = 0; iCnt < 9; ++iCnt)
    {
        nY = iSectorY + dY[iCnt];
        nX = iSectorX + dX[iCnt];

        if (nY < dfRANGE_MOVE_TOP || dfRANGE_MOVE_BOTTOM < nY || nX < dfRANGE_MOVE_LEFT || dfRANGE_MOVE_RIGHT < nX)
            continue;
        
        pSectorAound->Around[iCount].iY = nY;
        pSectorAound->Around[iCount].iX = nX;
        iCount++;
    }
}

void C_Field::GetUpdateSectorAround(st_Player* pCharacter, st_SECTOR_AROUND* pRemoveSector, st_SECTOR_AROUND* pAddSector)
{
}

bool C_Field::Sector_UpdateCharacter(st_Player* pPlayer)
{
    int SectorY;
    int SectorX;

    SectorY = pPlayer->_Y / Grid_Y_Size;
    SectorX = pPlayer->_X / Grid_X_Size;

    // 실제 섹터와 저장되어 있는 섹터가 다르다면, 갱신한다. 
    if (pPlayer->_CurSector->iX != SectorX || pPlayer->_CurSector->iY != SectorY)
    {
        pPlayer->_OldSector = pPlayer->_CurSector;

        pPlayer->_CurSector->iY = SectorY;
        pPlayer->_CurSector->iX = SectorX;

        return true;
    }

    return false;
}

void C_Field::CharacterSectorUpdatePacket(st_Player* pPlayer)
{
    //--------------------------------------------
    // 8방 이동.
    // But 대각선 이동은, 수평이동 + 수직이동 임.
    //--------------------------------------------
    st_SECTOR_POS* st_Old_Sector = pPlayer->_OldSector;
    st_SECTOR_POS* st_Cur_Sector = pPlayer->_CurSector;
    st_PACKET_HEADER st_Header;

    // 위 방향 이동
         if (st_Old_Sector->iY > st_Cur_Sector->iY && st_Old_Sector->iX == st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 아래 방향 이동
    else if (st_Old_Sector->iY < st_Cur_Sector->iY && st_Old_Sector->iX == st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 왼쪽 방향 이동
    else if (st_Old_Sector->iY == st_Cur_Sector->iY && st_Old_Sector->iX > st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 오른쪽 방향 이동
    else if (st_Old_Sector->iY == st_Cur_Sector->iY && st_Old_Sector->iX > st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 좌측 상단
    else if (st_Old_Sector->iY < st_Cur_Sector->iY && st_Old_Sector->iX > st_Cur_Sector->iX) 
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 우측 상단
    else if (st_Old_Sector->iY < st_Cur_Sector->iY && st_Old_Sector->iX < st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 좌측 하단 
    else if (st_Old_Sector->iY > st_Cur_Sector->iY && st_Old_Sector->iX > st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
    // 우측 하단
    else if (st_Old_Sector->iY > st_Cur_Sector->iY && st_Old_Sector->iX < st_Cur_Sector->iX)
    {
        // 삭제하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_DELETE_CHATACTER;
        st_Header.bySize = 4;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        C_Session::GetInstance()->SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
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



