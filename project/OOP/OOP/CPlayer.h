#ifndef __CPLAYER_H__
#define __CPLAYER_H__


class CPlayer : CBaseObject 
{
private:
	char _cSkin;
	int _iX;
	int _iY;

public:
	CPlayer(int ObjectType, int iY, int iX, char cSkin);
	virtual ~CPlayer();

	virtual bool Update(void);
	virtual void Render(void);

	void KeyboardInput(void);
};


#endif // !__PLAYER_H__

