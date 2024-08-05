
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




//st_SECTOR_POS::st_SECTOR_POS() 
//{ 
//    iX = NULL;
//    iY = NULL;
//}

//st_SECTOR_POS::st_SECTOR_POS(int X, int Y)
//{
//    iX = X / C_Field::GetInstance()->Grid_X_Size;
//    iY = Y / C_Field::GetInstance()->Grid_Y_Size;
//}


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

    pSectorAound->iCount = iCount;
}

void C_Field::GetAttackSectorAround(int iCurSectorX, int iCurSectorY, int iX, int iY, char byDirection, int i_Attack_Range_X, int i_Attack_Range_Y, st_SECTOR_AROUND* pSectorAound)
{
    int nY;
    int nX;
    int iCnt;
    int iSectorY;
    int iSectorX;

    //------------------------------------------------------------
    // 자신이 있던 섹터는 무조건 공격 범위 섹터에 포함된다.
    pSectorAound->Around[0].iY = iCurSectorY;
    pSectorAound->Around[0].iX = iCurSectorX;
    iCnt = 1;

    if (byDirection == dfPACKET_MOVE_DIR_LL)
    {
        // 왼쪽 방향 공격

        // 왼쪽 방향 섹터 검색
        iSectorX = (iX - i_Attack_Range_X) / Grid_X_Size;
        if (iCurSectorX != iSectorX)
        {
            pSectorAound->Around[iCnt].iX = iSectorX;
            pSectorAound->Around[iCnt].iY = iCurSectorY;
            ++iCnt;

            // 왼쪽 방향 섹터 확정/ 위 아래 섹터 확인
            iSectorY = (iY + i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;

                // 2중 if() 문으로 대각선에 위치한 섹터 구함
                pSectorAound->Around[iCnt].iX = iSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }

            iSectorY = (iY - i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;

                // 2중 if() 문으로 대각선에 위치한 섹터 구함
                pSectorAound->Around[iCnt].iX = iSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }
        }
        else
        {
            // 오른쪽 방향이 다른 섹터로 넘어가지 않는다면, 위 아래 방향의 섹터만 탐색해보면 된다.
            iSectorY = (iY + i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }

            iSectorY = (iY - i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }
        }
    }
    else
    {
        // 오른쪽 방향 공격
        
        // 오른쪽 방향 섹터 검색
        iSectorX = (iX + i_Attack_Range_X) / Grid_X_Size;
        if (iCurSectorX != iSectorX)
        {
            pSectorAound->Around[iCnt].iX = iSectorX;
            pSectorAound->Around[iCnt].iY = iCurSectorY;
            ++iCnt;

            // 오른쪽 방향 섹터를 탐색해야 하는 상태에서 위쪽 혹은 아랫쪽 섹터도 탐색해야 하는지 확인
            iSectorY = (iY + i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt; 

                // 2중 if() 문으로 대각선에 위치한 섹터 구함
                pSectorAound->Around[iCnt].iX = iSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }

            iSectorY = (iY - i_Attack_Range_Y) / Grid_Y_Size;
            if(iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;

                // 2중 if() 문으로 대각선에 위치한 섹터 구함
                pSectorAound->Around[iCnt].iX = iSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }
        }
        else
        {
            // 오른쪽 방향이 다른 섹터로 넘어가지 않는다면, 위 아래 방향의 섹터만 탐색해보면 된다.
            iSectorY = (iY + i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }

            iSectorY = (iY - i_Attack_Range_Y) / Grid_Y_Size;
            if (iCurSectorY != iSectorY)
            {
                pSectorAound->Around[iCnt].iX = iCurSectorX;
                pSectorAound->Around[iCnt].iY = iSectorY;
                ++iCnt;
            }
        }
    }

    pSectorAound->iCount = iCnt

}

void C_Field::GetUpdateSectorAround(st_PLAYER* pCharacter, st_SECTOR_AROUND* pRemoveSector, st_SECTOR_AROUND* pAddSector)
{
}

bool C_Field::Sector_UpdateCharacter(st_PLAYER* pPlayer)
{
    int SectorY;
    int SectorX;

    SectorY = pPlayer->_Y / Grid_Y_Size;
    SectorX = pPlayer->_X / Grid_X_Size;

    // 실제 섹터와 저장되어 있는 섹터가 다르다면, 갱신한다. 
    if (pPlayer->_CurSector->iX != SectorX || pPlayer->_CurSector->iY != SectorY)
    {
        //----------------------------------------------------------------
        // 이런식으로 동적할당 된 변수를 '=' 연산하면 문제가 발생함
        // pPlayer->_OldSector = pPlayer->_CurSector;
        pPlayer->_OldSector->iY = pPlayer->_CurSector->iY;
        pPlayer->_OldSector->iX = pPlayer->_CurSector->iY;

        pPlayer->_CurSector->iY = SectorY;
        pPlayer->_CurSector->iX = SectorX;

        return true;
    }

    return false;
}

void C_Field::CharacterSectorUpdatePacket(st_PLAYER* pPlayer)
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
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
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
        SendPacket_SectorOne(st_Old_Sector->iX + 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX + 0, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Old_Sector->iX - 1, st_Old_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();

        // 생성하는 메시지 전송
        st_Header.byCode = (char)dfPACKET_CODE;
        st_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;
        st_Header.bySize = 10;
        g_Packet.PutData((char*)&st_Header, sizeof(st_Header));
        g_Packet << pPlayer->_SessionID << pPlayer->_byDirection << pPlayer->_X << pPlayer->_Y << pPlayer->_HP;
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY - 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 0, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX + 0, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        SendPacket_SectorOne(st_Cur_Sector->iX - 1, st_Cur_Sector->iY + 1, &g_Packet, NULL);
        g_Packet.Clear();
    }
}

void C_Field::removeUserFromSector(DWORD dwSessionID, st_SECTOR_POS* pSector_Pos)
{
    g_Sector_Hash[pSector_Pos->iY][pSector_Pos->iX].erase(dwSessionID);
}

void C_Field::SendPacket_SectorOne(int iSectorX, int iSectorY, SerializeBuffer* pPacket, st_SESSION* pExceptSession)
{
    std::unordered_map<DWORD, st_PLAYER*> ::iterator iter;

    // 섹터에 대한 예외 처리
    if (!Check_Sector_CoordinateRange(iSectorX, iSectorY))
        return;

    if (pExceptSession == NULL)
    {
        for (iter = g_Sector_Hash[iSectorY][iSectorX].begin(); iter != g_Sector_Hash[iSectorY][iSectorX].end(); ++iter)
        {
            SendPacket_Unicast((*iter).second->_pSession, pPacket);
        }
    }
    else
    {
        for (iter = g_Sector_Hash[iSectorY][iSectorX].begin(); iter != g_Sector_Hash[iSectorY][iSectorX].end(); ++iter)
        {
            if (pExceptSession == (*iter).second->_pSession)
            {
                continue;
            }

            SendPacket_Unicast((*iter).second->_pSession, pPacket);
        }
    }
}

void C_Field::SendPacket_Around(st_SESSION* pSession, SerializeBuffer* pPacket, st_SECTOR_AROUND* pSector_Around, bool bSendMe)
{
    int iCnt;

    if (bSendMe == false)
    {
        for (iCnt = 0; iCnt < pSector_Around->iCount; ++iCnt)
        {
            SendPacket_SectorOne(pSector_Around->Around[iCnt].iX, pSector_Around->Around[iCnt].iY, pPacket, pSession);
        }
    }
    else
    {
        for (iCnt = 0; iCnt < pSector_Around->iCount; ++iCnt)
        {
            SendPacket_SectorOne(pSector_Around->Around[iCnt].iX, pSector_Around->Around[iCnt].iY, pPacket, NULL);
        }
    }
}

bool C_Field::Check_Sector_CoordinateRange(int iSectorX, int iSectorY)
{
    if (iSectorX < 0 || iSectorY < 0 || iSectorX > _Sector_Max_X || iSectorY > _Sector_Max_Y)
        return false;
    return true;
}

st_PLAYER* C_Field::GetPlayerInSector(int iSectorX, int iSectorY)
{
    // 현재 섹터 해쉬가 잘못된거 같음. 하나의 플레이어 포인터만을 저장하고 있다.
}

void st_SECTOR_POS::Init_SECTOR_POS(int Y, int X)
{
    iY = Y / dfGRID_Y_SIZE;
    iX = X / dfGRID_X_SIZE;
}

C_Field::C_Field()
{
    //int iCntX;
    //int iCntY;
    int Sector_Max_X;
    int Sector_Max_Y;

    _Sector_Max_Y = Sector_Max_Y = dfRANGE_MOVE_BOTTOM / Grid_Y_Size;
    _Sector_Max_X = Sector_Max_X = dfRANGE_MOVE_RIGHT / Grid_X_Size;


    // 3차원 배열: 면, 행, 열 순서
    // 2차원 배열: 행, 열 순서
    // 1차원 배열: 열 순서
    //_Sector = new std::list<st_PLAYER*>**[Sector_Max_Y];
    //for (iCntY = 0; iCntY < Sector_Max_Y; ++iCntY)
    //{
    //    _Sector[iCntY] = new std::list<st_PLAYER*>*[Sector_Max_X];
    //    for (iCntX = 0; iCntX < Sector_Max_X; ++iCntX)
    //    {
    //        _Sector[iCntY][iCntX] = new std::list<st_PLAYER*>();
    //    }
    //}
}

C_Field::~C_Field()
{
    //int iCntY;
    //int iCntX;
    int Sector_Max_X;
    int Sector_Max_Y;

    Sector_Max_Y = _Sector_Max_Y;
    Sector_Max_X = _Sector_Max_X;

    //for (iCntY = 0; iCntY < _Sector_Max_Y; ++iCntY)
    //{
    //    for (iCntX = 0; iCntX < _Sector_Max_X; ++iCntX)
    //    {
    //        // 동적으로 할당된 st_Player들은 Player 클래스에서 어련히 잘 알아서 메모리를 해지했다고 가정한다. 
    //        // 각 std::list<st_Player*> 해제
    //        delete _Sector[iCntY][iCntX];
    //    }
    //    // 열 배열 해제
    //    delete[] _Sector[iCntY];
    //}
    //// 행 배열 해제
    //delete[] _Sector;
}



