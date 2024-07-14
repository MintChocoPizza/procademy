
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

	st_Player(DWORD SessionID, st_SESSION* pSession);
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
	// Key: SessionID, Value: st_Player*
	std::unordered_map< DWORD, st_Player*>	_CharacterMap;

};


#endif // !__PALYER_H__
