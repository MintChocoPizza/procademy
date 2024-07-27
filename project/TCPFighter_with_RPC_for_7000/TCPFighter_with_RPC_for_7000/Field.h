
//--------------------------------------------------------------------------------------------------------------------
// 필드 클래스를 만든 이유:
// 
// 유저들의 위치를 필드 단위로 설정하는 방법:
//	1. 모든 유저들의 x,y 좌표를 그리드 단위로 나누어 해당하는 그리드를 위치를 구해낸다.  
//		단점: 패킷을 보낼 때 마다, 모든 유저들을 대상으로 x,y 좌표를 기준으로 연산하여 그리드 위치를 구해내야 한다. 
// 
//	2. 생성 이동, 등등 플레이어의 움직임이 있을 때 마다, 해당 그리드에 포함되는 플레이어들을 저장해둔다 .
//		플레이어들의 위치가 필요할 때 마다, 그냥 해당 그리드를 탐색하면 해당 위치에 있는 플레이어를 한번에 알 수 있다. 
//--------------------------------------------------------------------------------------------------------------------

#ifndef __FIELD_H__
#define __FIELD_H__

using namespace std;

extern int dY[];
extern int dX[];

#define dfGRID_X_SIZE 100
#define dfGRID_Y_SIZE 100

#define dfSECTOR_MAX_Y dfRANGE_MOVE_BOTTOM / dfGRID_Y_SIZE
#define dfSECTOR_MAX_X dfRANGE_MOVE_RIGHT  / dfGRID_X_SIZE

struct st_SECTOR_AROUND;
struct st_Player;

class C_Field
{
public:
	enum
	{
		Grid_X_Size = 100,
		Grid_Y_Size = 100
	};

public:
	

public:
	static C_Field* GetInstance(void);

	//---------------------------------------------------------------------------------------------
	// 특정 섹터 좌표 기준 주변 영향권 섹터 얻기
	// SectorX, SectorY의 섹터좌표에서 주변 최대 9개의 섹터 좌표를 얻어 오는 함수
	void GetSectorAround(int iSectorX, int iSectorY, st_SECTOR_AROUND* pSectorAound);

	//---------------------------------------------------------------------------------------------
	// 캐릭터가 섹터를 이동 한 뒤에 영향권에서 빠진 섹터, 새로 추가된 섹터의 정보 구하는 함수
	void GetUpdateSectorAround(st_Player* pCharacter, st_SECTOR_AROUND* pRemoveSector, st_SECTOR_AROUND* pAddSector);

	//---------------------------------------------------------------------------------------------
	// 섹터가 변경된 것을 확인한다.
	bool Sector_UpdateCharacter(st_Player* pPlayer);
	//---------------------------------------------------------------------------------------------
	// 변경된 섹터에 대해서 메시지를 보낸다. 
	void CharacterSectorUpdatePacket(st_Player* pPlayer);


private:
	C_Field();
	~C_Field();

private:
	static C_Field _C_Field;

public:
	// Key: <GridY, GridX>, Value: SessionID
	// std::unordered_multimap<std::pair<int, int>, DWORD> _Grid;
	// 그냥 동적할당 배열을 사용하여 섹터당 유저들을 저장한다. 
	std::list<st_Player*>*** _Sector;
	std::list<st_Player*> g_Sector[dfSECTOR_MAX_Y][dfSECTOR_MAX_X];

	int _Sector_Max_Y;
	int _Sector_Max_X;
};

#endif // !__FIELD_H__

struct st_SECTOR_POS
{
	int iX;
	int iY;

	st_SECTOR_POS();
	st_SECTOR_POS(int X, int Y);

};

struct st_SECTOR_AROUND
{
	int iCount;
	st_SECTOR_POS Around[9];
	st_SECTOR_AROUND();
};

