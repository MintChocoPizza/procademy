
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
	st_SECTOR_POS* _CurSector;
	st_SECTOR_POS* _OldSector;

	st_Player(DWORD SessionID, st_SESSION* pSession);
	~st_Player();
};

// Key: SessionID, Value: st_Player*
extern std::unordered_map< DWORD, st_Player*>	g_CharacterHash;

void ClearCharacterHash(void);
st_Player* FindCharacter(DWORD dwSessionID);
st_Player* CreateNewPlayer(DWORD SessionID, st_SESSION* st_p_New_Session);



#endif // !__PALYER_H__
