
#ifndef __PALYER_H__
#define __PALYER_H__


class C_Player
{
public:
	C_Player();
	C_Player(DWORD SessionID);
	~C_Player();

public:
	DWORD		_SessionID;
	DWORD		_dwAction;
	BYTE		_byDirection;
	short		_X;
	short		_Y;
	char		_HP;

	bool		_Disconnect;
};


#endif // !__PALYER_H__
