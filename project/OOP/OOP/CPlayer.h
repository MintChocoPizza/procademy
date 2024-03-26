#ifndef __CPLAYER_H__
#define __CPLAYER_H__


class CPlayer : CBaseObject 
{
public:
	CPlayer(int ObjectType);
	virtual ~CPlayer();

	virtual bool Update(void);
};


#endif // !__PLAYER_H__

