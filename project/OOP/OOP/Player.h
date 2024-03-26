#ifndef __PLAYER_H__
#define j__PLAYER_H__


class Player : CBaseObject 
{
public:
	Player(int ObjectType);
	virtual ~Player();

	virtual bool Update(void);
};


#endif // !__PLAYER_H__

