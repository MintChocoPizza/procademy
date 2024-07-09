
#ifndef __PALYER_H__
#define __PALYER_H__

struct st_SECTOR_POS;

struct st_Player
{
	st_SESSION* _pSession;
	DWORD		_SessionID;
	DWORD		_dwAction;
	BYTE		_byDirection;
	BYTE		_byModeDirection;	// 아마도 움직이던 방향

	st_SECTOR_POS* _SectorPos;

	short		_X;
	short		_Y;

	char		_HP;
	bool		_Disconnect;

	// 구현에 따라 다르다.
	// CurSector;
	// OldSector;

	st_Player(DWORD SessionID, st_SESSION* pSession)
	{
		_pSession = pSession;
		_SessionID = SessionID;
		_Disconnect = false;
		_HP = 100;
		_dwAction = -1;

		// rand() % 최대값 + 최솟값
		// 0 < _Y < 6401
		// 0 < _X < 6401
		_Y = rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP + 1) + dfRANGE_MOVE_TOP;
		_X = rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT + 1) + dfRANGE_MOVE_LEFT;

		_SectorPos = new st_SECTOR_POS(_Y, _X);

		_SectorPos->iY = _Y / C_Field::GetInstance()->Grid_Y_Size;
		_SectorPos->iX = _X / C_Field::GetInstance()->Grid_X_Size;
		
		_byDirection = (rand() % 8) < 4 ? 0 : 4;
		_byModeDirection = -1;
	}
};

class C_Player
{
public:
	static C_Player* GetInstance(void);
	st_Player* CreateNewPlayer(DWORD SessionID, st_SESSION* st_p_New_Session);

private:
	C_Player();
	~C_Player();
private:
	static C_Player _C_Player;


public:
	std::unordered_map<DWORD, st_Player*>	_CharacterMap;

};


#endif // !__PALYER_H__
