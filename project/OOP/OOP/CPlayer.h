#ifndef __CPLAYER_H__
#define __CPLAYER_H__


class CPlayer : CBaseObject 
{
private:
	int _iHP;

	char _cSkin;
	char _cBulletSkin;

	int _iDamage;

	int _iX;
	int _iY;

	int _iPlayerSpeed;
	int _iBulletCoolTime;



public:
	CPlayer(int ObjectType, char* PlayerFile);
	virtual ~CPlayer();

	virtual bool Update(void);
	virtual void Render(void);

	void KeyboardInput(void);
};


#endif // !__PLAYER_H__

