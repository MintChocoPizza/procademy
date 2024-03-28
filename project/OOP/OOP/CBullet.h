#ifndef __CBULLET_H__
#define __CBULLET_H__


class CBullet : public CBaseObject
{
private:
	char _Skin;
	int _iY;
	int _iX;

	int _dY;
	int _dX;

	int _iCoolTime;
	
	

public:
	CBullet();
	~CBullet();
};


#endif // !__CBULLET_H__

